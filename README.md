# templater
__templater__ is a command-line tool for automated placeholder substitution in a template with values from a data file.

### Data file
* Each non-comment line defines a variable in the form `KEY=VALUE`.
* Leading and trailing whitespace, whitespace around `=` are ignored.
* Lines beginning with `#` or `//` are ignored.
* `KEY` and `VALUE` may contain only Latin letters, digits, and underscores.
* `KEY` and `VALUE` may each contain at most 100 characters.
* The file may contain at most 1,024 `KEY=VALUE` pairs.
* The last `VALUE` for the `KEY` is used.

### Template file
* Placeholders have the form `{{ KEY }}`.
* Whitespace inside the braces is ignored.
* Placeholder’s value is taken from the data file.
* Every valid placeholder is replaced with the corresponding `VALUE`.


### Usage
| Short     | Long              | Required | Description                                  |
|-----------|-------------------|----------|----------------------------------------------|
| `-d PATH` | `--data=PATH`     | Yes      | Path to the data file.                       |
| `-t PATH` | `--template=PATH` | Yes      | Path to the template file.                   |
| `-o PATH` | `--output=PATH`   | No       | Path to the output file. Defaults to `stdout`. |

Example:
```console
$ templater --template=template.txt --data=data.dat --output=output.txt
```

### Exit statuses
* 0 -- Success.
* 1 -- The template contains a placeholder with no corresponding key.
* 2 -- Invalid command-line arguments.
* 3 -- I/O error.
* 4 -- Syntax error.
* 5 -- Other error.
