#!/bin/bash

exec valgrind --leak-check=full --show-leak-kinds=all ./interpreter
