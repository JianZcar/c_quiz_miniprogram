{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell rec {
  buildInputs = [
    pkgs.gcc                # GCC Compiler
    pkgs.cjson              # cJSON library (this package includes both header and library)
    pkgs.pkg-config          # For detecting libraries like cJSON during compile
  ];

  # Optional: You may want to set some environment variables here
  shellHook = ''
    echo "Welcome to the quiz build environment!"

    # Correct path to cJSON.h
    CJSON_INCLUDE_DIR="${pkgs.cjson}/include/cjson"
    CJSON_LIB_DIR="${pkgs.cjson}/lib"

    echo "cJSON include directory: $CJSON_INCLUDE_DIR"
    echo "cJSON library directory: $CJSON_LIB_DIR"

    # Set flags for compiling with cJSON
    export CFLAGS="-I$CJSON_INCLUDE_DIR"
    export LDFLAGS="-L$CJSON_LIB_DIR -lcjson"

    # Define the 'build' function for compiling the project
    build() {
      echo "Building the quiz program..."

      # Ensure the directories exist
      if [ ! -f src/main.c ]; then
        echo "Error: src/main.c not found!"
        return 1
      fi

      if [ ! -f data/questions.json ]; then
        echo "Error: data/questions.json not found!"
        return 1
      fi

      # Explicitly set the include path and library path
      gcc -o quiz src/main.c $CFLAGS $LDFLAGS -lcjson

      if [ $? -eq 0 ]; then
        echo "Build successful! You can now run the program using ./quiz"
      else
        echo "Build failed. Please check for errors."
      fi
    }

  '';
}
