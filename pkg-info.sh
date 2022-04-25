#! bash -uv
scoop info emacs | awk 'NF' | awk '$1=="Bucket"{print $3}'
