package main

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
)

var suffix = [...]string{
	".c", ".h", ".cpp", ".hpp", ".cc", ".hxx", ".C", ".c++",
	".asm", ".s", ".S",
	".cs",
	".java", ".kt",
	".go", ".py", ".php",
	".js", ".ts", ".css", ".htm", ".html",
	".vue", ".json", ".xml", ".yml",
	"makefile", "Makefile", "GNUMakefile",".md"}

var TT = 0

var NN = 0

func doFile(path string) {
	i:=1
	for _, suf := range suffix {
		if strings.HasSuffix(path, suf) {
			i=0
			break
		}
	}
	if i==1 {
		return
	}
	file, e := os.Open(path)
	if e != nil {
		fmt.Println(e)
		return
	}
	var N = 1
	var T = 0
	buf := make([]byte, 1024)
	for {
		r, _ := file.Read(buf)
		T += r
		if r == 0 {
			break
		}
		for _, b := range buf {
			if b == '\n' {
				N++
			}
		}
	}
	TT += T
	NN += N
	var c = 0
	if T >= 1024 {
		T = T / 1024
		c = 'K'
	}
	fmt.Printf("%8d行\t%8d %cB\t (%s)\n", N, T, c, path)
}

func main() {
	var files []string
	err := filepath.Walk("./", func(path string, info os.FileInfo, err error) error {
		files = append(files, path)
		return nil
	})
	if err != nil {
		panic(err)
	}

	for _, file := range files {
		doFile(file)
	}
	fmt.Printf("总共：%d行，%dKB\n", NN, TT)
}
