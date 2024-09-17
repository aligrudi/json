#!/bin/sh
# This file demonstrates how to use json.

# initializing an example input file:
cat >test.txt <<EOF
{
	"count": 3,
	"path": "/home",
	"parent": {"path": "/", "size": "325M"},
	"data": [
		{"name": "user", "size": "2M", "children": 40},
		{"name": "john", "size": "8M", "children": 5},
		{"name": "test", "size": "1M", "children": 13}
	]
}
EOF

# printing fields of objects:
./json count <test.txt
# 3

# printing fields of nested objects:
./json parent.size <test.txt
# 325M

# printing multiple fields of an object (fields are tab-separated):
./json path,count <test.txt
# /home	3

# printing nested entries and fields:
./json data.0 <test.txt
# {"name": "user", "size": "2M", "children": 40}

# list entries can be accessed by index:
./json data.1.name <test.txt
# john

# printing all entries:
./json 'data.*.name' <test.txt
# user
# john
# test

# printing multiple fields of all list entries:
./json 'data.*.size,name' <test.txt
# 2M	user
# 8M	john
# 1M	test
