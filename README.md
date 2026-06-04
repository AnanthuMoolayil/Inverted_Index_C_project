[![C Project CI](https://github.com/AnanthuMoolayil/Inverted_Index_C_project/actions/workflows/ci.yml/badge.svg)](https://github.com/AnanthuMoolayil/Inverted_Index_C_project/actions/workflows/ci.yml)                                                                       

# Inverted_Index_C_project
Inverted index implementation which performs the following:
1. Create a hash lookup table mapping words to document ids containing these words.
2. Display results for input search queries.

## Features
1. Single word query
2. Chained Boolean(AND/OR) query
3. Tokenization & normalization
4. Stop word removal during indexing
5. Punctuation stripping from file being indexed and input query
6. Invalid query handling
7. Verified leak-free via Address Sanitiser
   
## Hash table visualisation
<img width="700" height="600" alt="hash_table_layout" src="https://github.com/user-attachments/assets/a12829d2-dcc6-433a-ab25-0e56e9dda81f" />
<br>
<br>


## Build

```bash
gcc -std=c17 \
    -Wall -Wextra \
    -Wshadow \
    -Wconversion \
    -Wsign-conversion \
    -g \
    -fsanitize=address,undefined \
    1_source_files/*.c -o search_engine
```

## Run (Example Query)

```bash
./search_engine cat AND dog
Search results:
file_1
```

## Output 
Here is a sample screenshot of input queries:<br>
<img width="812" height="502" alt="sample_queries" src="https://github.com/user-attachments/assets/99f7a51d-5341-4429-96ec-61b7f6ef80de" />
<br>
<br>
Here is a sample screenshot of invalid query handling:<br>
<img width="812" height="598" alt="invalid_queries" src="https://github.com/user-attachments/assets/c0770e09-e2d9-43d5-904b-b22b7f2f90fc" />

## Concepts used:
### A. Core Data structures
  1. **Hash table**:<br>
     a. Hash table function<br>
     b. Bucketed storage<br>
     c. Collision traversal<br>
     d. Lookup optimization<br>
  2. **Linked List**:<br>
     a. Insertion, deletion and traversal<br>
     b. Nested linked lists<br>
  3. **Multi level dynamic structures**:<br>
     a. Nested structures<br>
     b. Relationship modeling<br>

### B. Memory management<br>
  1. **Memory resource handling**:<br>
     a. Allocation error handling<br>
     b. Cleanup logic<br>
  2. **Debugging**:<br>
     a. Address sanitiser usage<br>
     b. Buffer overflow & linked list edge case handling<br>

### C. Algorithm<br>
  1. **Hash table indexing and inserting**:<br>
     a. Pointer manipulation<br>
     b. Code structural flow<br>
  2. **Input query parsing**:<br>
     a. Tokenization & normalization<br>
     b. Input validation & sanitization<br>
  3. **Precomputation for faster search output**<br>

### D. Architecture & Design<br>
  1. **Modular design**:<br>
     a. Separate structures<br>
     b. Helper fuunctions<br>
     c. Abstraction & Encapsulation<br>
  2. **Error Handling**:<br>
     a. Library style design<br>
     b. NULL checks & boundary validation<br>
  3. **Testing & validation**:<br>
     a. Debugging approach<br>
     b. Query validation<br>


     
