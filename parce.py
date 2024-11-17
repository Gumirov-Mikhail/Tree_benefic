import pandas as pd
import os
import warnings

# Игнорировать все предупреждения
warnings.filterwarnings("ignore")

# Или игнорировать только `DeprecationWarning`
warnings.filterwarnings("ignore", category=DeprecationWarning)

# Создаем папку для сохранения обработанных данных
output_dir = "data_new"
os.makedirs(output_dir, exist_ok=True)

# Загрузка файла TSV в DataFrame
file_path_1 = os.getenv("DATA_COMPANY", "data/company.tsv")
file_path_2 = os.getenv("DATA_FOUNDER_LEGAL", "data/founder_legal.tsv")
file_path_3 = os.getenv("DATA_FOUNDER_NATURAL", "data/founder_natural.tsv")
company = pd.read_csv(file_path_1, sep="\t", quoting=3)
founder_legal = pd.read_csv(file_path_2, sep="\t", quoting=3)
founder_natural = pd.read_csv(file_path_3, sep="\t")

#company valid
#удаление, если строки невалидные
all_filled = company.notna().sum(axis=1) != 4
company = company[~all_filled]

#удаление, если повторки среди ogrn, inn и full_name
company = company[~company['inn'].duplicated(keep = False)]
company = company[~company['full_name'].duplicated(keep = False)]
company = company[~company['ogrn'].duplicated(keep = False)]

#delete, if ogrn or inn = 0, and full_name.empty
company = company[company['inn'] != 0]
company = company[company['ogrn'] != 0]
company = company[company['full_name'].str.len() > 0]



#founder_natural valid
#if len(str) < 6 => delete
all_filled = founder_natural.notna().sum(axis=1) < 6
founder_natural = founder_natural[~all_filled]

#replace nan to 0
founder_natural = founder_natural.fillna(0)

#delete, if inn = 0 or company_id = 0
founder_natural = founder_natural[founder_natural['inn'] != 0]
founder_natural = founder_natural[founder_natural['company_id'] != 0]

#delete, if inns don't differents, but FIO differents
#Объединение FIO в одну колонку
founder_natural['fio_name'] = (
    founder_natural['last_name'] + " " +
    founder_natural['first_name'] + " " +
    founder_natural['second_name']
)

# Удаление строк, где одинаковые 'inn', но разные FIO
founder_natural = founder_natural.groupby('inn').filter(
    lambda x: x['fio_name'].nunique() == 1
)

founder_natural.drop(columns=['fio_name'], inplace=True)


#founder_legal valid
#if len(str) < 5 => delete
all_filled = founder_legal.notna().sum(axis=1) < 5
founder_legal = founder_legal[~all_filled]

#delete, if ogrn or inn = 0, and full_name.empty
founder_legal = founder_legal[founder_legal['inn'] != 0]
founder_legal = founder_legal[founder_legal['ogrn'] != 0]
founder_legal = founder_legal[founder_legal['full_name'].str.len() > 0]


#replace nan to 0
founder_legal = founder_legal.fillna(0)



# Функция для обработки каждой группы по `ogrn`
def process_group_inn(group):
    # Подсчитываем количество каждого `inn` в группе
    inn_counts = group['inn'].value_counts()

    # Находим максимальную частоту
    max_count = inn_counts.max()

    # Отбираем все `inn` с этой частотой
    frequent_inns = inn_counts[inn_counts == max_count].index

    # Если несколько `inn`, выбираем тот с максимальным share
    if len(frequent_inns) > 1:
        max_share_inn = (
            group[group['inn'].isin(frequent_inns)]
            .groupby('inn')['share']
            .sum()
            .idxmax()
        )
    else:
        max_share_inn = frequent_inns[0]

    # Оставляем все строки с выбранным `inn`
    return group[group['inn'] == max_share_inn]

# Применяем функцию к каждой группе по `ogrn`
founder_legal = founder_legal.groupby('ogrn', group_keys=False).apply(process_group_inn)



def process_group_ogrn(group):
    # Подсчитываем количество каждого `ogrn` в группе
    ogrn_counts = group['ogrn'].value_counts()

    # Находим максимальную частоту
    max_count = ogrn_counts.max()

    # Отбираем все `ogrn` с этой частотой
    frequent_ogrns = ogrn_counts[ogrn_counts == max_count].index

    # Если несколько ogrn встречаются одинаковое количество раз, выбираем тот, у которого максимальный share
    if len(frequent_ogrns) > 1:
        max_share_ogrn = (
            group[group['ogrn'].isin(frequent_ogrns)]
            .groupby('ogrn')['share']
            .sum()
            .idxmax()
        )
    else:
        max_share_ogrn = frequent_ogrns[0]

    # Возвращаем только строки с наиболее частым ogrn
    return group[group['ogrn'] == max_share_ogrn]

# Применяем функцию к данным, группируя по 'inn' (а не 'ogrn')
founder_legal = founder_legal.groupby('inn', group_keys=False).apply(process_group_ogrn)


#преобразуем всё в одно название, самое длинное
founder_legal['longest_full_name'] = (
    founder_legal.groupby(['ogrn', 'inn'])['full_name']
    .transform(lambda names: max(names, key=len))
)

# Обновляем колонку full_name
founder_legal['full_name'] = founder_legal['longest_full_name']

# Удаляем вспомогательную колонку
founder_legal.drop(columns=['longest_full_name'], inplace=True)



#output
# Сохранение результата
company.to_csv(os.path.join(output_dir, "company.tsv"), sep="\t", index=False)
founder_natural.to_csv(os.path.join(output_dir, "founder_natural.tsv"), sep="\t", index=False)
founder_legal.to_csv(os.path.join(output_dir, "founder_legal.tsv"), sep="\t", index=False)

