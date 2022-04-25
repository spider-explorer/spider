#! bash
set -e
python cpplint.py \
--filter=-legal/copyright,\
-build/include_subdir,\
-readability/casting,\
-whitespace/parens,\
-build/include_what_you_use,\
-runtime/references,\
-whitespace/braces,\
-whitespace/newline,\
-whitespace/line_length,\
-whitespace/indent,\
-build/header_guard,\
-whitespace/comments,\
-whitespace/operators,\
-build/include_order,\
 main/*.cpp main/*.h
