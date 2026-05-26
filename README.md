# Inverted_Index_C_project
Inverted index implementation which performs the following:
1. Create a hash lookup table mapping words to document ids containing these words.
2. Display results for Boolean AND/OR chain input queries.

## Build

```bash
gcc *.c -o search_engine
```

## Run (Example Query)

```bash
./search_engine cat AND dog
```

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


     
