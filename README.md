# Misis Coconut

## How to run?
Insure that you have docker installed on your system.

1. Pull docker image
```bash
docker pull zxcry/biv_hack
```

2. Run docker
```bash
docker run --rm -v <path_to_data>:/app/data zxcry/biv_hack
```
Your data should be in the following structure
```
data/
├── company.tsv
├── founder_legal.tsv
└── founder_natural.tsv
```
Output will be placed in the current directory

## Project Description
We developed an algorithm for automated data processing to identify relationships between counterparties and clean input data from errors and duplicate records.  

Our project includes:  
- **Data Filtering**: Removing invalid rows and eliminating duplicate records based on analysis of INN and OGRN.  
- **Name Optimization**: Standardizing company names to the most complete and accurate version.  
- **Relationship Mapping**: Building a graph to visualize and analyze connections between companies.  
- **Infrastructure**: Deploying the solution in Docker containers for scalability and ease of use.  

This project automates and simplifies working with large data volumes, ensuring high accuracy and reliability of results.
