# Jaccard Distance

A C implementation of the Jaccard distance algorithm using custom data structures and graph processing techniques.

## Features

- Jaccard distance computation
- Graph representation
- Hash table implementation
- Dynamic container (Holdall)
- File parsing utilities

## Technologies

- C
- GCC
- Make

## Project Structure

```text
jaccard-distance/
├── doc/
│   └── report.pdf
├── jaccard/
│   ├── Makefile
│   ├── main.c
│   ├── distance.c
│   ├── distance.h
│   ├── graph.c
│   ├── graph.h
│   ├── hashtable.c
│   ├── hashtable.h
│   ├── holdall.c
│   ├── holdall.h
│   ├── lecture.c
│   ├── lecture.h
│   └── ...
├── Makefile
├── README.md
├── LICENSE
└── .gitignore
```

## Build

Compile the project:

```bash
make -C jaccard
```

## Clean

```bash
make clean
```

## Create the project archive

```bash
make dist
```

This command generates the archive:

```text
jaccard-distance.tar.gz
```

## Documentation

The complete project report is available in:

```text
doc/report.pdf
```

## Learning Objectives

This project demonstrates:

- Programming in C
- Data structures
- Hash tables
- Graph processing
- Similarity measurement using the Jaccard distance

## Author

**Kenza Yousfi**

Bachelor's Degree in Computer Science  
University of Rouen Normandy

## License

This project is released under the MIT License.
