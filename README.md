# Hide-and-Seek Game Theory

A Hide and Seek game based on **game theory principles**, developed in **C** using **SDL2**.

---

## How to Use

### 1. Build the project (compile only)
```bash
./hideseek.sh build
````

### 2. Build and run the project

```bash
./hideseek.sh run
```

### 3. Clean build files

```bash
./hideseek.sh clean
```

---

## Important Notes

* To **use the seeker or hider**, and to **find their hiding or guessing probabilities**, follow this process:

  1. Call the initialize function to create a seeker or hider with zeroed probabilities:

  ```c
  struct seeker initialize_seeker(int num_chests, int **payoff_matrix);
  struct hider initialize_hider(int num_chests, int **payoff_matrix);
  ```

  2. Use the `find_*_strategy` function to calculate the probabilities:

  ```c
  struct seeker find_seeker_strategy(struct seeker *seeker, int num_chests);
  struct hider find_hider_strategy(struct hider *hider, int num_chests);
  ```

  * Input the initialized seeker/hider.
  * It returns a new seeker/hider with the calculated probabilities.

* To **choose a chest based on probabilities**, use this function from `logic.c`:

```c
int choose_chest(int num_chests, int *probabilities);
```

It returns the chest number selected according to the probabilities.

---

## Main Files and Components

* **seeker.h** — Functions and structures for the seeker.
* **hider.h** — Functions and structures for the hider.
* **logic.h** — Core game logic, including the `choose_chest` function.

These three are the main pillars to integrate your game logic with the GUI.

---
