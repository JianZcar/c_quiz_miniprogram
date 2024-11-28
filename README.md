# Quiz Program

This is a simple command-line quiz program written in C that loads questions and answers from a JSON file. The program presents multiple-choice questions to the user, shuffles the answer options, and evaluates the user's responses.

## Features

- Load questions and answers from a JSON file.
- Shuffle questions and answer options for each quiz session.
- Evaluate user input and provide feedback on correctness.
- Display the final score at the end of the quiz.

## Requirements

- GCC (GNU Compiler Collection)
- cJSON library
- pkg-config (for library detection)

## Installation

1. **Clone the repository:**

   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

2. **Set up the development environment using Nix:**

   If you have Nix installed, you can enter the development shell with the required dependencies:

   ```bash
   nix-shell
   ```

3. **Build the project:**

   Inside the Nix shell, run the build command:

   ```bash
   build
   ```

   This will compile the program and create an executable named `quiz`.

## JSON File Structure

The program expects a JSON file located at `data/questions.json`. The structure of the JSON file should be as follows:

```json
[
    {
        "question": "What is the capital of France?",
        "answers": ["Paris", "London", "Berlin", "Madrid"],
        "right_answer": "Paris"
    },
    {
        "question": "What is 2 + 2?",
        "answers": ["3", "4", "5", "6"],
        "right_answer": "4"
    }
    // Add more questions as needed
]
```

## Running the Program

After building the program, you can run it using:

```bash
./quiz
```

Follow the prompts to answer the questions. Enter the letter corresponding to your answer (A, B, C, or D).

## License

This project is licensed under the MIT License. See the LICENSE file for more details.

## Acknowledgments

- [cJSON](https://github.com/DaveGamble/cJSON) for the JSON parsing library.
- Any other libraries or resources you used.
