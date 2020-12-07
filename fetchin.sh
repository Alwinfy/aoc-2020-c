#!/bin/sh

[ -z "$1" ] && {
	printf "Usage: $0 <days>\n"
	exit 1
}

keyfile=session.key

session=$(cat "$keyfile")

for arg in "$@"; do
	day=$(printf "%02d" $arg)
	printf "Fetching day $day... "
	curl -fs -H"Cookie: session=$session" "https://adventofcode.com/2020/day/$arg/input" > "day$day.in" && printf "done.\n" || printf "errored!\n"
done
