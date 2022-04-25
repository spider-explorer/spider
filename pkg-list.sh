#! bash
scoop search | awk 'NF' | awk '{print $1}'
