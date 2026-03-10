#!/usr/bin/env python3
# KIWAKU - Containers Well Made
# Copyright : KIWAKU Project Contributors
# SPDX-License-Identifier: BSL-1.0
import re
import sys
import argparse

def process_text(text, prefix):
  """
  Identifies types starting with _:: and replaces them with
  '{prefix}_implementation_defined', correctly handling C++ template
  bracket balancing (< >).

  Test Cases:
  >>> # Basic replacement with default prefix
  >>> process_text("_::simple_type", prefix="kwk")
  'kwk_implementation_defined'

  >>> # Basic replacement of explicit namespace
  >>> process_text("kwk::_::simple_type", prefix="kwk")
  'kwk_implementation_defined'

  >>> # Handling prefix operators
  >>> process_text("!_::value_of<X>", prefix="kwk")
  '!kwk_implementation_defined'

  >>> # Handling complex templates with custom prefix
  >>> process_text("_::some_code<T, U, 4 >", prefix="std")
  'std_implementation_defined'

  >>> # Handling nested templates
  >>> process_text("_::vector<_::allocator<int>>* ptr", prefix="kwk")
  'kwk_implementation_defined* ptr'

  >>> # Handling multiple occurrences
  >>> process_text("void f(_::type1 a, _::type2 b)", prefix="kwk")
  'void f(kwk_implementation_defined a, kwk_implementation_defined b)'
  """
  pattern = re.compile(r'([a-zA-Z0-9_]+::)?_::[a-zA-Z0-9_]+')
  replacement_token = f"{prefix}_implementation_defined"
  offset = 0
  result = []

  while True:
    match = pattern.search(text, offset)
    if not match:
      result.append(text[offset:])
      break

    result.append(text[offset:match.start()])

    remaining = text[match.end():]
    if remaining.startswith('<'):
      count = 0
      idx = 0
      for char in remaining:
        if char == '<':
          count += 1
        elif char == '>':
          count -= 1
        idx += 1
        if count == 0:
          break

      result.append(replacement_token)
      offset = match.end() + idx
    else:
      result.append(replacement_token)
      offset = match.end()

  return "".join(result)

def main():
  parser = argparse.ArgumentParser(description="Filter C++ implementation details for Doxygen.")
  parser.add_argument("file", nargs="?", help="Input file to process")
  parser.add_argument("--test", action="store_true", help="Run doctests")
  parser.add_argument("-p", "--prefix", help="Prefix for the replacement string")

  args = parser.parse_args()

  if args.test:
    import doctest
    result = doctest.testmod(optionflags=doctest.NORMALIZE_WHITESPACE)
    if result.failed == 0:
      print(f"Success: All {result.attempted} tests passed.")
    sys.exit(0)

  if not args.file:
    parser.print_help()
    sys.exit(1)

  try:
    with open(args.file, 'r') as f:
      content = f.read()

    print(process_text(content, prefix=args.prefix), end='')
  except FileNotFoundError:
    sys.stderr.write(f"Error: File {args.file} not found.\n")
    sys.exit(1)

if __name__ == "__main__":
  main()
