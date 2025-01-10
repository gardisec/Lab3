package main

import (
	"GO/hashtable"
	"fmt"
	"log"
)

func main() {
	hashTable := hashtable.NewHashTable(10)
	err := hashTable.Push("biba", "boba")
	if err != nil {
		log.Fatalf("Error. can't push: %v", err)
	}

	err = hashTable.Push("boba", "biba")
	if err != nil {
		log.Fatalf("Error. can't push: %v", err)
	}

	err = hashTable.Push("aboba", "52")
	if err != nil {
		log.Fatalf("Error. can't push: %v", err)
	}

	value, err := hashTable.Get("biba")
	if err != nil {
		log.Fatalf("Error. can't get: %v", err)
	}
	fmt.Println("Finded:", value)

	err = hashTable.Delete("biba")
	if err != nil {
		log.Fatalf("Error. can't delete: %v", err)
	}

	err = hashTable.SaveToFile("hashtable.txt")
	if err != nil {
		log.Fatalf("Error. can't save to file: %v", err)
	}

	err = hashTable.LoadFromFile("hashtable.txt")
	if err != nil {
		log.Fatalf("Error. can't load file: %v", err)
	}
	err = hashTable.SaveToBinaryFile("hashtable.bin")
	if err != nil {
		log.Fatalf("Error. can't save to binary file: %v", err)
	}

	err = hashTable.LoadFromBinaryFile("hashtable.bin")
	if err != nil {
		log.Fatalf("Error. can't load binary file: %v", err)
	}

	hashTable.Print()
}
