package main

import (
	"fmt"
	"os"
)

const ROUTINE_COUNT = 3

var n = [][]int{
	[]int{1, 2, 3},
	[]int{0, 5, 0},
	[]int{7, 7, 7},
}

var m = [][]int{
	[]int{2, 2, 4},
	[]int{0, 6, 3},
	[]int{3, 1, 1},
}

var result_matrix = [][]int{
	[]int{0, 0, 0},
	[]int{0, 0, 0},
	[]int{0, 0, 0},
}

func usage() {
	fmt.Printf("Usage: %s <operation>\n", os.Args[0])
}

func add(row int, done chan bool) {
	for i := row; i < len(n); i += ROUTINE_COUNT {
		for j := 0; j < len(n[i]); j++ {
			result_matrix[i][j] = n[i][j] + m[i][j]
		}
	}

	done <- true
}

func subtract(row int, done chan bool) {
	for i := row; i < len(n); i += ROUTINE_COUNT {
		for j := 0; j < len(n[i]); j++ {
			result_matrix[i][j] = n[i][j] - m[i][j]
		}
	}

	done <- true
}

func multiply(row int, done chan bool) {
	for i := row; i < len(n); i += ROUTINE_COUNT {
		for j := 0; j < len(n[i]); j++ {
			result_matrix[i][j] = n[i][j] * m[i][j]
		}
	}

	done <- true
}

func output() {
	fmt.Printf("[ ")
	for i, j := 0, len(result_matrix); i < j; i++ {
		for k, l := 0, len(result_matrix[i]); k < l; k++ {
			fmt.Printf("%d ", result_matrix[i][k])
		}

		if i != (j - 1) {
			fmt.Printf("\n  ")
		}
	}
	fmt.Printf("]\n")
}

func main() {
	var f func(row int, done chan bool)
	chs := []chan bool{}

	if len(os.Args) != 2 {
		usage()
		return
	}

	switch os.Args[1] {
	case "add":
		f = add
	case "subtract":
		f = subtract
	default:
		f = multiply
	}

	for i := 0; i < ROUTINE_COUNT; i++ {
		chs = append(chs, make(chan bool))
		go f(i, chs[i])
		<-chs[i]
	}

	output()
}
