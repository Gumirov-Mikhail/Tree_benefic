FROM python:3.9

ENV DATA_NEW "data_new/"
WORKDIR /app
COPY . .

RUN pip install --no-cache-dir -r requirements.txt

RUN apt-get update && apt-get install -y build-essential
RUN apt install -y g++ libfmt-dev && apt-get clean

RUN g++ /app/src/main.cpp -o /app/main -lfmt

CMD ["bash", "-c", "python3 ./parce.py && ./main"]
