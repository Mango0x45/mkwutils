#!/usr/bin/env sh

printf I | od -to2 | awk '{ print substr($2, 6, 1); exit }'
