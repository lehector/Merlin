# Merlin SyGuS Solver

## GETTING STARTED
### Option 1: Build Docker Image

Download the code from here and do this on the command line:
```bash
    docker build . -t merlin
```
Within the docker image, the software is located here: `/artifact`.


### Option 2: Manual installation

  0. Install prerequisites as in Docker File
  1. Download the code from here.
  2. Build the application:
     ```bash
       cd path/to/artifact
       mkdir build
       cd build
       cmake .. -DCMAKE_BUILD_TYPE=RELEASE
       make
     ```

## Implementation Structure
The folder `ast` contains classes implementing literals, variables and operations, mimicking the SMT implementation.
The folder `benchmarks` contains a benchmarks suite adapted from PROBE.
The folder `Tests` contains a test suite (for the ast objects).
The folders `thirdparty` and `antlr4-runtime` contain foreign code.
The folder `SMTProcesses` contains implementations of the class `SMTProcess.h`. 
They handle interaction with the SMT Solver. 
The class `SyGuSFileTask` handles the interaction with the parser and creates many instances of `VocabMaker` put into the `VocabFactory` which organizes the makers into leaf makers and non-leave makers.
A `VocabMaker` is used to generate an `ASTNode` or to retrieve the function of an operation.
The class `IOExample` represents an input-output example.
The class `Helper` contains useful definitions and helper functions.
The entry point for the program is `main.cpp` which in turn creates a `Solver`.
The `Solver` implements the CEGIS Loop interacting with an enumerator from the folder `enumeration`.
The folder `enumeration` contains different implementations of enumerators, e.g. height-based and size-based, different orimetrics, etc.

## Running Merlin
To run Merlin, execute the following command:
```bash
./build/merlin --input-file="./benchmarks/blaze/bikes.sl" --check-violation 1 --cutoff-size 3 --n-values-check 3 --reuse-components 1 --use-oe 1 --max-contexts 1 --domain="string" --silent 1
```

For more usage information, you can run Merlin with the `--help` argument.
```bash
./merlin --help                                                                                                                                                                   
 
Allowed options:
  --help                        produce help message
  --run-example arg             whether to run the example
  -s [ --silent ] arg           whether to use silent mode, default = 0
  -i [ --input-file ] arg       input file
  -v [ --check-violation ] arg  check violation if enabled
  -o [ --use-oe ] arg           use observational equivalence, default = 1
  -c [ --cutoff-size ] arg      cut off size, for check violation
  -m [ --max-contexts ] arg     how many contexts should be considered in first
                                iteration
  -n [ --n-values-check ] arg   how many values to check before creating object
                                instance, default = 3
  -r [ --reuse-components ] arg reuse components if enabled
  -d [ --domain ] arg           domain: either "string" or "bv"
  --replace-complete arg        be complete with replace, default = 0
  --and-or-radius arg           radius for and / or, default = 100
  --mul-radius arg              radius for mul, default  = 100
  --hd-radius arg               radius for hd, default = 66
  --concat-substr-radius arg    radius for concat / substr, default = 100
  --levenshtein-radius arg      radius for levenshtein, default = 4
```
