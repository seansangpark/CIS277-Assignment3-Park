# CIS-277 Assignment 3: Text Messages and Dynamic Memory

This package supplies public checks, not a student solution or a grade. Use the assignment instructions to build your `MessageInbox` with dynamic linked nodes.

## Add the checker to your public assignment repository

Copy these three files into the matching paths in your repository:

```text
validate.py
tests/public_tests.cpp
.github/workflows/validate.yml
```

Keep your own student `README.md`. This instruction file is packaged as `README-PREFLIGHT.md`; it must not replace your student README.

The checker also expects your `MessageInbox.h`, `MessageInbox.cpp`, `main.cpp`, and `README.md` at the repository root.

## Run the checker

From the repository root, using a C++23-capable compiler:

```sh
python3 validate.py --compiler g++
```

On Windows, you can use `python` instead of `python3`. GCC or clang can be selected with `--compiler` or `CXX`. For current MSVC, run this in a Visual Studio Developer Command Prompt:

```text
python validate.py --compiler cl
```

Optional memory checks with GCC/clang and installed sanitizer runtimes:

```sh
python3 validate.py --compiler g++ --sanitize
```

This runner does not support `--sanitize` with MSVC.

## Student README headings

Write your own content under these exact headings in `README.md`:

```markdown
# CIS-277 Assignment 3: Text Messages and Dynamic Memory
## Student
## Design
## How to Compile
## How to Run
## Tests and Results
## Analysis Questions
## AI Use
```

Headings inside a fenced example do not count. The checker verifies that the headings exist; your explanations, answers, test evidence, and AI-use disclosure need instructor review.

## What the checker covers

- Required files, README headings, public signatures, and a header that compiles independently.
- Arrival order, unread/read flags, missing IDs, duplicates, invalid input, and unchanged state on rejection.
- Text-byte accounting, exact budget limits, deletion at every list location, freed-budget reuse, and reusing a deleted ID.
- Copied input, independent snapshots, repeated reads, repeated clear, reuse, and destruction with stored messages.
- Compilation of your demonstration in C++23 mode. Run and review your demonstration yourself; the checker does not execute it.

The budget checks use ASCII text and charge only `text.size()` bytes. Sender names and node overhead are not part of this simulated text-byte budget.

## Reading the result

- **PASS / exit 0:** public checks completed. Review all assignment requirements before submitting.
- **FAIL / exit 1:** missing files/headings, a compile or runtime error, a failed check, or a timeout. Correct the reported problem and rerun.
- **INCOMPLETE / exit 2:** the compiler command is missing/invalid or the requested sanitizer option is unsupported. Set up your compiler and rerun.

The checker builds in a temporary directory. It does not write build artifacts into your repository. Manual linked-node design, allocation/deletion, complexity, explanation quality, and AI-use compliance require separate review. Public checks and a green GitHub workflow do not establish a final grade.

The included **Assignment 3 Preflight** workflow runs on pushes, pull requests, and manual dispatch. No implementation is included in this package.
