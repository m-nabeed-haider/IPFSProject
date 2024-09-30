#pragma once
#include <iostream>
#include<string>
#include"Btree.h"
#include"File.h"
using namespace std;

//=====================MACHINE=========================
template <typename T>
class Machine {
public:
	T id;
	BTree<T>* btree;
	Machine* next;
	Machine** routing_table;

	Machine(T id, int identifier)
	{
		this->id = id;
		next = NULL;
		routing_table = new Machine * [identifier + 1];
		btree = new BTree<T>(2);
	}
};


//=========================DHT RING======================
template <typename T>
class DHT_Ring
{
	Machine<T>* head;
	int identifier;
public:
	DHT_Ring(int identifier)
	{
		head = NULL;
		this->identifier = identifier;

	}
	void addMachine(T id, bool& status)
	{
		Machine<T>* newMachine = new Machine<T>(id, identifier);

		if (head == NULL)
		{
			head = newMachine;
			head->next = head;

		}
		else
		{
			Machine<T>* nodeptr = head;
			if (nodeptr->id > id)
			{
				newMachine->next = head;
				while (nodeptr->next != head)
				{
					nodeptr = nodeptr->next;
				}
				nodeptr->next = newMachine;
				head = newMachine;

				return;
			}

			while (nodeptr->next != head && nodeptr->next->id <= id)
			{
				nodeptr = nodeptr->next;
			}

			if (nodeptr->id == id)
			{
				//cout << "machine of this id already exist \n";
				status = false;
				return;
			}

			Machine<T>* temp = nodeptr->next;
			nodeptr->next = newMachine;
			newMachine->next = temp;

		}
	}
	//=========================linking routing nodes==================
	void setFTS()
	{
		int count = 1;
		Machine<T>* current = head;
		T temp_id;

		while (current->next != head)
		{
			count = 1;

			while (count <= identifier)
			{
				temp_id = current->id + T(pow(2, count - 1));
				temp_id = temp_id % T(pow(2, identifier));
				Machine<T>* temp = search(temp_id);
				current->routing_table[count] = temp;

				count++;
			}
			current = current->next;
		}

		count = 1;
		while (count <= identifier)
		{
			temp_id = current->id + T(pow(2, count - 1));
			temp_id = temp_id % T(pow(2, identifier));
			Machine<T>* temp = search(temp_id);
			current->routing_table[count] = temp;

			count++;
		}



	}
	//=====================search by circular ring O(N)===================
	Machine<T>* search(T id)
	{
		if (head == NULL)
		{
			return NULL;
		}
		else
		{
			Machine<T>* nodeptr = head;
			while (nodeptr->next != head && nodeptr->next->id < id)
			{
				nodeptr = nodeptr->next;
			}

			return nodeptr->next;
		}

	}
	//==================search by routing table  (Log N)=============
	Machine<T>* search_data(T id)
	{
		Machine<T>* current = head;
		if (head == NULL)
		{
			return NULL;
		}
		else
		{

			if (id <= current->id)
			{
				cout << "SEARCHING ID IS :" << current->id << endl;
				return current;
			}
			if (id <= current->routing_table[1]->id)
			{
				cout << "SEARCHING ID IS :" << current->routing_table[1]->id << endl;
				return current->routing_table[1];
			}

			while (current->next != head)
			{
				int count = 0;
				while (count < this->identifier)
				{
					if (current->routing_table[count + 1]->id > id)
					{
						break;
					}
					if (current->routing_table[count + 1]->id <= current->id)
					{
						break;
					}
					count++;
				}
				//count -= 1;
				current = current->routing_table[count];
				if (current->id == id)
				{
					break;
				}

				if (current->routing_table[1]->id >= id)
				{
					cout << "SEARCHING ID IS :" << current->routing_table[1]->id << endl;
					return current->routing_table[1];
					//break;
				}
			}
			if (current->next == head && current->id < id)
			{
				current = current->next;
			}

			cout << "SEARCHING ID IS :" << current->id << endl;
			return current;
		}
		return head;
	}
	//==========================================
	void insert_data(T temp_key, string key, string path)
	{

		Machine<T>* valid_machine = search_data(temp_key);

		valid_machine->btree->insert(temp_key);

		BTreeNode<T>* node = valid_machine->btree->search_node(temp_key);

		int i = 0;
		while (i < node->count && temp_key > node->keys[i])
			i++;
		node->data[i] = new File(path);
		node->data[i]->key = key;
		node->data[i]->readFromFile();
		valid_machine->btree->display(valid_machine->btree->getRoot());

	}
	//-------------------------------------------------------------
	void delete_data(T temp_key)
	{

		Machine<T>* valid_machine = search_data(temp_key);

		valid_machine->btree->deletion(temp_key);
		valid_machine->btree->display(valid_machine->btree->getRoot());


	}
	//----------------------------------------------------------------
	void delete_machine(T key)
	{
		if (head->id == key)
		{
			Machine<T>* temp = this->head;
			Machine<T>* temp2 = this->head;

			while (temp2->next != head)
			{
				temp2 = temp2->next;
			}

			head = head->next;
			temp2->next = head;

			data_exchange(temp->btree->root, 0);
			delete temp;
			temp = NULL;
			setFTS();
		}
		else
		{
			if (head->next == head)
			{
				head = NULL;
				return;
			}
			Machine<T>* temp = this->head;
			while (temp->next != head && temp->next->id != key)
			{
				temp = temp->next;
			}

			Machine<T>* actual = temp->next;
			Machine<T>* next_ofactual = temp->next->next;
			temp->next = next_ofactual;

			data_exchange(actual->btree->root, 0);
			delete actual;
			actual = NULL;
			setFTS();
		}

	}
	void data_exchange(BTreeNode<T>* x, int level = 0)
	{

		if (x != nullptr)
		{

			for (int i = 0; i < x->count; i++)
			{

				insert_data(x->keys[i], x->data[i]->key, x->data[i]->path);

			}

			if (!x->isLeaf) {
				for (int i = 0; i <= x->count; i++) {
					data_exchange(x->children[i], level + 1);
				}
			}
		}

	}
	//---------------------------------------------------
	void insertMachine(T key)
	{
		Machine<T>* newMachine = new Machine<T>(key, identifier);
		Machine<T>* temp = head;
		if (head == NULL)
		{
			head = newMachine;
			head->next = head;
			return;
		}
		else
		{
			while (temp->next != head && temp->next->id < key)
			{
				temp = temp->next;
			}
		}

		newMachine->next = temp->next;
		temp->next = newMachine;
		setFTS();
		data_exchange_insert(newMachine->next->btree->root, newMachine->id, 0);
	}

	void data_exchange_insert(BTreeNode<T>* x, T id, int level = 0)
	{

		if (x != nullptr)
		{

			for (int i = 0; i < x->count; i++)
			{
				if (x->keys[i] <= id)
				{
					insert_data(x->keys[i], x->data[i]->key, x->data[i]->path);
				}

			}

			if (!x->isLeaf) {
				for (int i = 0; i <= x->count; i++) {
					data_exchange_insert(x->children[i], id, level + 1);
				}
			}
		}

	}
	//==============================================================
	void print()
	{
		Machine<T>* nodeptr = head;
		if (nodeptr)
		{

			while (nodeptr->next != head)
			{
				cout << nodeptr->id << " -> ";

				for (int i = 1; i <= this->identifier; i++)
				{
					cout << nodeptr->routing_table[i]->id << " -> ";
				}
				cout << endl;
				nodeptr = nodeptr->next;
			}
			cout << nodeptr->id << " -> ";
			for (int i = 1; i <= this->identifier; i++)
			{
				cout << nodeptr->routing_table[i]->id << " -> ";
			}
			cout << endl;
			cout << "NULL" << endl;
		}
	}

};

//=====================