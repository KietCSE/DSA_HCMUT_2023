#include "main.h"

extern int MAXSIZE;

class imp_res : public Restaurant {
private: 
	class Node {
	public:
		customer* data; 
		int time; 
		Node* next; 
		Node* prev; 
		Node() : data(nullptr), time(0), next(nullptr), prev(nullptr){}
		Node(string name, int energy, int t, Node* p = nullptr, Node* ptr = nullptr) {
			data = new customer(name, energy, nullptr, nullptr); 
			time = t; 
			prev = p; 
			next = ptr; 
		}
		~Node() {
			delete data; 
			next = nullptr; 
			prev = nullptr;
		}
	};


	class Queue {
	private:
		Node* pHead;
		Node* pTail;
		int count;
	protected:
		// can dam bao end la mot node thuoc hang doi
		void IncreaseStep(Node*& current, Node* end, int step) {
			for (int i = 0; i < step && current != end; i++) current = current->next;
		}

		void DecreaseStep(Node*& current, Node* start, int step) {
			for (int i = 0; i < step && current != nullptr && current != start; i++) current = current->prev;
		}

		int SortSegment(Node* start, Node* end, int step) {
			// current la node muon chen
			Node* current = start;
			IncreaseStep(current, end, step);
			// hai con tro chay lui
			Node* after = current;
			Node* before = start;
			int Num = 0;
			while (current != end) {
				// selection
				while (after != start && (abs(before->data->energy) < abs(after->data->energy))
					|| (abs(before->data->energy) == abs(after->data->energy) && after->time < before->time)) {
					// swap
					
					customer* temp = after->data; 
					after->data = before->data; 
					before->data = temp;
					
					int TIME = after->time;
					after->time = before->time;
					before->time = TIME;
					Num++;

					DecreaseStep(after, start, step);
					DecreaseStep(before, start, step);
				}
				IncreaseStep(current, end, step);
				after = current;
				before = current;
				DecreaseStep(before, start, step);
			}
			return Num;
		}

	public:
		Queue() : pHead(nullptr), pTail(nullptr), count(0) {}
		
		~Queue() {
			Node* current;
			while (this->pHead != nullptr) {
				current = pHead;
				pHead = pHead->next;
				//delete current->data;
				delete current;
			}
			pHead = nullptr;
			pTail = nullptr;
			count = 0;
		}

		void Enqueue(string name, int energy, int time) {
			if (this->count == MAXSIZE) return;
			if (pHead == nullptr) {
				pHead = new Node(name, energy, time);
				pTail = this->pHead;
			}
			else {
				pTail->next = new Node(name, energy, time, pTail, nullptr);
				pTail = pTail->next;
			}
			count++;
		}

		// moi them vao
		void Enqueue(Node* cus) {
			if (cus == nullptr) return;
			if (pHead == nullptr) {
				pHead = cus;
				cus->prev = nullptr; 
				cus->next = nullptr;
				pTail = this->pHead;
			}
			else {
				pTail->next = cus; 
				cus->next = nullptr;
				cus->prev = pTail; 
				pTail = pTail->next;
			}
			count++;
		}

		Node* Dequeue() {
			if (pHead == nullptr) return nullptr;
			Node* temp = pHead;
			pHead = pHead->next;
			if (pHead) pHead->prev = nullptr;
			count--;
			if (count == 0) pTail = nullptr;
			temp->next = nullptr;
			temp->prev = nullptr;
			return temp;
		}

		int ShellSort() {
			int NumSwap = 0;
			// tim vi tri dung lai 
			Node* end = pHead; 
			for (Node* p = pHead; p != nullptr; p = p->next) {
				if (abs(p->data->energy) > abs(end->data->energy) 
				|| (abs(p->data->energy) == abs(end->data->energy) && p->time > end->time)) {
					end = p;
				}
			}
			
			if (end == nullptr) return -1; // truong hop hang doi rong

			// dem so phan tu can sap xep
			int cnt = 1;
			Node* a = pHead;
			while (a != end) { cnt++; a = a->next; }

			end = end->next;

			// duyet cac step
			for (int step = cnt / 2; step > 2; step /= 2) {
				Node* p = this->pHead;
				// duyet cac day con
				for (int j = 0; j < step; j++) {
					NumSwap += SortSegment(p, end, step);
					p = p->next;
				}
			}
			NumSwap += SortSegment(pHead, end, 1);  // stable tai doan nay
			return NumSwap;
		}

		bool isEmpty() {
			return count == 0;
		}

		int Size() { return this->count; }

		bool ContainName(string name) {
			Node* p = pHead; 
			while (p != nullptr) {
				if (p->data->name == name) return true; 
				p = p->next; 
			}
			return false;
		}

		void RemoveAt(Node* current) {
			if (current == nullptr) return;
			if (current == pHead) {
				pHead = current->next;
				if (pHead) pHead->prev = nullptr;
				if (pHead == nullptr) pTail = nullptr;
				delete current;
			}
			else {
				current->prev->next = current->next;
				if (current->next) current->next->prev = current->prev;
				if (pTail == current) pTail = current->prev;
				delete current;
			}
			this->count--;
		}

		// moi them vao
		Node* ReturnRemoveAt(Node* current) {
			if (current == nullptr) return nullptr;
			if (current == pHead) {
				pHead = current->next;
				if (pHead) pHead->prev = nullptr;
				if (pHead == nullptr) pTail = nullptr;
				this->count--;
				current->next = nullptr;
				current->prev = nullptr;
				return current;
			}
			else {
				current->prev->next = current->next;
				if (current->next) current->next->prev = current->prev;
				if (pTail == current) pTail = current->prev;
				this->count--;
				current->next = nullptr;
				current->prev = nullptr;
				return current;
			}
			return nullptr;
		}

		Node* MinTimeElementOL() {
			if (pHead == nullptr) return nullptr;
			Node* move = this->pHead; 
			Node* position = nullptr; 
			int MinTime = 100000000;
			while (move != nullptr) {
				if (move->data->energy < 0 && move->time < MinTime) {
					MinTime = move->time; 
					position = move; 
				}
				move = move->next;
			}
			return position;
		}

		Node* MinTimeElementCTS() {
			if (pHead == nullptr) return nullptr;
			Node* move = this->pHead;
			Node* position = nullptr;
			int MinTime = 100000000;
			while (move != nullptr) {
				if (move->data->energy > 0 && move->time < MinTime) {
					MinTime = move->time;
					position = move;
				}
				move = move->next;
			}
			return position;
		}

		/*int SumOfEnergy() {
			int ans = 0; 
			for (Node* p = pHead; p != nullptr; p = p->next) {
				ans += p->data->energy;
			}
			return ans;
		}*/

		void Print() {
			Node* p = pHead; 
			while (p != nullptr) {
				p->data->print(); 
				p = p->next;
			}
		}

		void PrintReverse() {
			Node* p = pTail;
			while (p != nullptr) {
				p->data->print();
				p = p->prev;
			}
		}

		// Ham test
		/*void printtail() {
			Node* p = pTail;
			while (p != nullptr) {
				p->data->print();
				p = p->prev;
			}
		}

		void printhead() {
			Node* p = pHead;
			while (p != nullptr) {
				p->data->print();
				p = p->next;
			}
		}*/
	};

	

protected:
	
	Node* pCurrent;
	Queue* QueueOfCustomer;
	int time;
	int size;
	int ToltalOfCTS; 
	int ToltalOfOL; 


	bool isExistNameInTable(string name) {
		if (pCurrent == nullptr) return false;
		Node* p = pCurrent;
		do {
			if (p->data->name == name) return true;
			p = p->next;
		} while (p != nullptr && p != pCurrent);
		return false;
	}

	void AddClockWise(Node* cus, Node* start) {
		start->next->prev = cus;
		cus->next = start->next;
		cus->prev = start;
		start->next = cus;
		this->size++;
		pCurrent = start->next; // cap nhap lai vi tri X
	}

	void AddNonClockWise(Node* cus, Node* start) {
		start->prev->next = cus;
		cus->next = start; 
		cus->prev = start->prev; 
		start->prev = cus;
		this->size++;
		pCurrent = start->prev; // cap nhap lai vi tri X
	}

	void clearTable() {
		if (pCurrent) {
			Node* p = pCurrent->next;
			Node* temp = nullptr;
			while (p != nullptr && p != pCurrent) {
				temp = p;
				p = p->next;

				//moi them vao 
				if (temp->data->energy < 0) ToltalOfOL -= temp->data->energy;
				else ToltalOfCTS -= temp->data->energy;

				delete temp;
			}
			//delete pCurrent->data; 
			if (pCurrent->data->energy < 0) ToltalOfOL -= pCurrent->data->energy;
			else ToltalOfCTS -= pCurrent->data->energy;

			delete pCurrent;
			pCurrent = nullptr;
			this->size = 0;
		}
	}

	void Remove(Node* position) {
		if (pCurrent == nullptr) return;

		if (this->size == 1) {
			//delete position->data;
			delete position;
			pCurrent = nullptr;
			this->size = 0;
		}
		else if (this->size == 2) {
			pCurrent = position->next; // cap nhap lai con tro pCurrent
			//delete position->data;
			delete position;
			pCurrent->next = nullptr;
			pCurrent->prev = nullptr;
			this->size--;
		}
		else {
			position->prev->next = position->next;
			position->next->prev = position->prev;
			// cap nhap lai vi tri pCurrent 
			if (position->data->energy > 0) pCurrent = position->next;
			else pCurrent = position->prev;
			delete position;
			this->size--;
		}
	}

	// moi them vao
	Node* ReturnRemove(Node* position) {
		if (pCurrent == nullptr) return nullptr;

		if (this->size == 1) {
			//delete position->data;
			pCurrent = nullptr;
			this->size = 0;
			position->next = nullptr; 
			position->prev = nullptr; 
			return position;
		}
		else if (this->size == 2) {
			pCurrent = position->next; // cap nhap lai con tro pCurrent
			//delete position->data;
			pCurrent->next = nullptr;
			pCurrent->prev = nullptr;
			this->size--;
			position->next = nullptr; 
			position->prev = nullptr;
			return position;
		}
		else {
			position->prev->next = position->next;
			position->next->prev = position->prev;
			// cap nhap lai vi tri pCurrent 
			if (position->data->energy > 0) pCurrent = position->next;
			else pCurrent = position->prev;
			this->size--;
			position->next = nullptr;
			position->prev = nullptr;
			return position;
		}
	}

	void PushFromQueue(Node* cus)
	{
		if (this->pCurrent == nullptr) {
			if (MAXSIZE == 0) return;
			pCurrent = cus;
			this->size++;
		}
		// tao thanh vong tron
		else if (this->size == 1) {
			if (this->size == MAXSIZE) return;
			pCurrent->prev = cus;
			pCurrent->next = cus;
			cus->next = pCurrent; 
			cus->prev = pCurrent;
			this->size++;
			pCurrent = pCurrent->next; // cap nhap lai vi tri X
		}
		else if (this->size < MAXSIZE / 2) {
			// them cung chieu kim dong do
			if (cus->data->energy >= pCurrent->data->energy) {
				this->AddClockWise(cus, pCurrent);
			}
			// them nguoc chieu kim dong ho
			else {
				this->AddNonClockWise(cus, pCurrent);
			}
		}
		else if (this->size >= MAXSIZE / 2 && this->size < MAXSIZE) {
			Node* position = pCurrent;
			Node* index = pCurrent;
			int RES = abs(position->data->energy - cus->data->energy);

			// tim vi tri co RES lon nhat 
			do {
				if (abs(position->data->energy - cus->data->energy) > RES) {
					index = position;
					RES = abs(position->data->energy - cus->data->energy);
				}
				position = position->next;
			} while (position != nullptr && position != pCurrent);

			// them vao ban an
			if (cus->data->energy - index->data->energy > 0) {
				this->AddClockWise(cus, index);
			}
			else {
				// them nguoc chieu kim dong ho
				this->AddNonClockWise(cus, index);
			}
		}
	}

	Node* MinTimeElementOL() {
		if (pCurrent == nullptr) return nullptr;
		Node* move = pCurrent;
		Node* table = nullptr;
		int MinTime = 100000000;
		do {
			if (move->data->energy < 0 && move->time < MinTime) {
				table = move;
				MinTime = move->time;
			}
			move = move->next;
		} while (move != nullptr && move != pCurrent);
		return table;
	}

	Node* MinTimeElementCTS() {
		if (pCurrent == nullptr) return nullptr;
		Node* move = pCurrent;
		Node* table = nullptr;
		int MinTime = 100000000;
		do {
			if (move->data->energy > 0 && move->time < MinTime) {
				table = move;
				MinTime = move->time;
			}
			move = move->next;
		} while (move != nullptr && move != pCurrent);
		return table;
	}

	/*int SumOfEnergy() {
		int ans = 0; 
		if (pCurrent == nullptr) return 0;
		Node* p = pCurrent; 
		do {
			ans += p->data->energy; 
			p = p->next;
		} while (p != nullptr && p != pCurrent);
		return ans;
	}*/

public:

	imp_res() : pCurrent(nullptr), time(0), size(0), ToltalOfCTS(0), ToltalOfOL(0) {
		QueueOfCustomer = new Queue();
	};

	~imp_res() {
		this->size = 0; 
		this->time = 0; 
		this->clearTable(); 
		this->ToltalOfCTS = 0;
		this->ToltalOfOL = 0;
		delete QueueOfCustomer;
	}

	void RED(string name, int energy)
	{
		if (energy == 0) return;
		if (this->isExistNameInTable(name)) return;
		if (QueueOfCustomer->ContainName(name)) return;

		if (this->pCurrent == nullptr) {
			if (MAXSIZE == 0) return;
			pCurrent = new Node(name, energy, this->time, nullptr, nullptr);
			this->size++;

			// moi them vao
			if (energy < 0) ToltalOfOL += energy;
			else ToltalOfCTS += energy;
		}
		// tao thanh vong tron
		else if (this->size == 1) {
			if (this->size == MAXSIZE) return;
			Node* cus = new Node(name, energy, this->time, pCurrent, pCurrent);
			pCurrent->prev = cus;
			pCurrent->next = cus;
			this->size++;
			pCurrent = pCurrent->next; // cap nhap lai vi tri X
			
			// moi them vao
			if (energy < 0) ToltalOfOL += energy;
			else ToltalOfCTS += energy;
		}
		else if (this->size < MAXSIZE / 2) {
			// them cung chieu kim dong do
			if (energy >= pCurrent->data->energy) {
				Node* cus = new Node(name, energy, this->time, nullptr, nullptr); 
				this->AddClockWise(cus, pCurrent);
			}
			// them nguoc chieu kim dong ho
			else {
				Node* cus = new Node(name, energy, this->time, nullptr, nullptr); 
				this->AddNonClockWise(cus, pCurrent);
			}
			
			// moi them vao
			if (energy < 0) ToltalOfOL += energy;
			else ToltalOfCTS += energy;
		}
		else if (this->size >= MAXSIZE / 2 && this->size < MAXSIZE) {
			Node* position = pCurrent;
			Node* index = pCurrent;
			int RES = abs(position->data->energy - energy);

			// tim vi tri co RES lon nhat 
			do {
				if (abs(position->data->energy - energy) > RES) {
					index = position;
					RES = abs(position->data->energy - energy);
				}
				position = position->next;
			} while (position != nullptr && position != pCurrent);

			// them vao ban an
			if (energy - index->data->energy > 0) {
				Node* cus = new Node(name, energy, this->time, nullptr, nullptr); 
				this->AddClockWise(cus, index);
			}
			else {
				// them nguoc chieu kim dong ho
				Node* cus = new Node(name, energy, this->time, nullptr, nullptr);
				this->AddNonClockWise(cus, index);
			}

			// moi them vao
			if (energy < 0) ToltalOfOL += energy;
			else ToltalOfCTS += energy;
		}
		// truong hop het ban thi day vao hang doi
		else {
			if (QueueOfCustomer->Size() < MAXSIZE) {
				this->QueueOfCustomer->Enqueue(name, energy, this->time);
				
				// moi them vao
				if (energy < 0) ToltalOfOL += energy;
				else ToltalOfCTS += energy;
			}
		}
		this->time++;
	}


	void BLUE(int num) {
		if (num == 0) return;
		if (pCurrent == nullptr) return;
		if (num >= this->size) {
			this->clearTable(); // da cap nhap total energy
		}
		else {
			for (int i = 0; i < num; i++) {
				// tim time nho nhat
				Node* move = pCurrent;
				Node* position = pCurrent;
				int MinTime = 100000000;
				do {
					if (move->time < MinTime) {
						position = move;
						MinTime = move->time;
					}
					move = move->next;
				} while (move != nullptr && move != pCurrent);
				
				// moi them vao
				if (position->data->energy < 0) ToltalOfOL -= position->data->energy;
				else ToltalOfCTS -= position->data->energy;

				this->Remove(position);
			}
		}

		while (!QueueOfCustomer->isEmpty() && this->size < MAXSIZE) {
			Node* another = QueueOfCustomer->Dequeue();
			this->PushFromQueue(another); // luu y cho nay 
		}
	}


	void PURPLE() {
		int NumSwap = QueueOfCustomer->ShellSort();
		NumSwap %= MAXSIZE;
		//cout << NumSwap << endl;
		this->BLUE(NumSwap);
	}

	void REVERSAL() {
		if (this->size <= 1) return;
		// dao nguoc so duong
		string name = pCurrent->data->name; 
		Node* temp = pCurrent;

		Node* front = pCurrent->next;
		Node* behind = pCurrent;
		while (front != behind) {
			while (front->data->energy < 0 && front != behind) front = front->next; 
			while (behind->data->energy < 0 && behind != front) behind = behind->prev;
			if (behind == front) break; // truong hop khong co gi de dao nguoc

			string temp = std::move(front->data->name); 
			front->data->name = std::move(behind->data->name); 
			behind->data->name = std::move(temp);

			int e = std::move(front->data->energy); 
			front->data->energy = std::move(behind->data->energy); 
			behind->data->energy = std::move(e);

			int t = front->time;
			front->time = behind->time;
			behind->time = t;

			front = front->next; 
			behind = behind->prev;
			if (front == behind->next) break;
		}
		// dao nguoc so am 
		front = pCurrent->next;
		behind = pCurrent;
		while (front != behind) {
			while (front->data->energy > 0 && front != behind) front = front->next;
			while (behind->data->energy > 0 && behind != front) behind = behind->prev;
			if (behind == front) break; // truong hop khong co gi de dao nguoc

			string temp = std::move(front->data->name);
			front->data->name = std::move(behind->data->name);
			behind->data->name = std::move(temp);

			int e = std::move(front->data->energy);
			front->data->energy = std::move(behind->data->energy);
			behind->data->energy = std::move(e);

			int t = front->time;
			front->time = behind->time;
			behind->time = t;

			front = front->next;
			behind = behind->prev;
			if (front == behind->next) break;
		}

		while (temp->data->name != name) {
			temp = temp->next; 
		}
		pCurrent = temp;
	}

	void UNLIMITED_VOID() {
		if (this->size < 4) return;
		/*if (this->size == 4) {
			Node* p = pCurrent->prev;
			do {
				p->data->print();
				p = p->next;
			} while (p != pCurrent->prev);
		}*/
		else {
			// current la node bat dau cua day con
			Node* current = pCurrent;
			int AnsSum = 1000000000;
			Node* start = nullptr;
			Node* end = nullptr;
			int len = 0;

			for (int i = 0; i < this->size; i++) {

				int Min = 100000000;
				int total = 0;
				Node* move = current;
				Node* idxmin = current;
				int length = 0;

				for (int i = 0; i < 3; i++) idxmin = idxmin->next; // luu vi tri ket thuc

				for (int j = 0; j < this->size; j++) {
					total += move->data->energy;
					if (j >= 3 && total <= Min) {
						Min = total;
						idxmin = move;
						length = j + 1;
					}
					move = move->next;
				}

				if (Min < AnsSum) {
					AnsSum = Min;
					start = current;
					end = idxmin;
					len = length;
				}
				else if (Min == AnsSum) {
					if (len <= length) {
						AnsSum = Min;
						start = current;
						end = idxmin;
						len = length;
					}
				}

				current = current->next;
			}
			// IN RA 
			Node* st = start;
			for (Node* it = start; true; it = it->next) {
				if (st->data->energy > it->data->energy) st = it;
				if (it == end) break;
			}

			for (Node* it = st; true; it = it->next) {
				it->data->print();
				if (it == end) break;
			}

			for (Node* it = start; st != start; it = it->next) {
				it->data->print();
				if (it->next == st) break;
			}

		}
	}


	//void DOMAIN_EXPANSION() {
	//	int Sum = this->SumOfEnergy() + QueueOfCustomer->SumOfEnergy(); 
	//	if (Sum >= 0) {
	//		// xoa oan linh
	//		Node* queue = QueueOfCustomer->MaxTimeElementOL();
	//		Node* table = this->MaxTimeElementOL();

	//		while (table != nullptr || queue != nullptr) {
	//			if (table == nullptr) {
	//				queue->data->print();
	//				QueueOfCustomer->RemoveAt(queue);
	//				queue = QueueOfCustomer->MaxTimeElementOL();
	//			}
	//			else if (queue == nullptr) {
	//				table->data->print();
	//				this->Remove(table);
	//				table = this->MaxTimeElementOL();
	//			}
	//			else if (table->time > queue->time) {
	//				table->data->print();
	//				this->Remove(table);
	//				table = this->MaxTimeElementOL();
	//			}
	//			else {
	//				queue->data->print();
	//				QueueOfCustomer->RemoveAt(queue);
	//				queue = QueueOfCustomer->MaxTimeElementOL();
	//			}
	//		}
	//	}
	//	else {
	//		// xoa chu thuat su 
	//		Node* queue = QueueOfCustomer->MaxTimeElementCTS();
	//		Node* table = this->MaxTimeElementCTS();

	//		while (table != nullptr || queue != nullptr) {
	//			if (table == nullptr) {
	//				queue->data->print();
	//				QueueOfCustomer->RemoveAt(queue);
	//				queue = QueueOfCustomer->MaxTimeElementCTS();
	//			}
	//			else if (queue == nullptr) {
	//				table->data->print();
	//				this->Remove(table);
	//				table = this->MaxTimeElementCTS();
	//			}
	//			else if (table->time > queue->time) {
	//				table->data->print();
	//				this->Remove(table);
	//				table = this->MaxTimeElementCTS();
	//			}
	//			else {
	//				queue->data->print();
	//				QueueOfCustomer->RemoveAt(queue);
	//				queue = QueueOfCustomer->MaxTimeElementCTS();
	//			}
	//		}
	//	}
	//	// them tu hang doi vao ban 
	//	while (!QueueOfCustomer->isEmpty() && this->size < MAXSIZE) {
	//		Node* another = QueueOfCustomer->Dequeue();
	//		this->PushFromQueue(another); 
	//	}
	//}

	void DOMAIN_EXPANSION() {
		/*int Sum = this->SumOfEnergy() + QueueOfCustomer->SumOfEnergy();*/
		int Sum = ToltalOfCTS - abs(ToltalOfCTS + ToltalOfOL);

		Queue* TempQueue = new Queue();
		
		if (Sum >= 0) {
			// xoa oan linh
			Node* queue = QueueOfCustomer->MinTimeElementOL();
			Node* table = this->MinTimeElementOL();

			while (table != nullptr || queue != nullptr) {
				if (table == nullptr) {
					Node* cus = QueueOfCustomer->ReturnRemoveAt(queue);
					TempQueue->Enqueue(cus);
					queue = QueueOfCustomer->MinTimeElementOL();
				}
				else if (queue == nullptr) {
					Node* cus = this->ReturnRemove(table);
					TempQueue->Enqueue(cus);
					table = this->MinTimeElementOL();
				}
				else if (table->time < queue->time) {
					Node* cus = this->ReturnRemove(table);
					TempQueue->Enqueue(cus);
					table = this->MinTimeElementOL();
				}
				else {
					Node* cus = QueueOfCustomer->ReturnRemoveAt(queue);
					TempQueue->Enqueue(cus);
					queue = QueueOfCustomer->MinTimeElementOL();
				}
			}
			// moi them vao 
			ToltalOfOL = 0;

		}
		else {
			// xoa chu thuat su 
			Node* queue = QueueOfCustomer->MinTimeElementCTS();
			Node* table = this->MinTimeElementCTS();

			while (table != nullptr || queue != nullptr) {
				if (table == nullptr) {
					Node* cus = QueueOfCustomer->ReturnRemoveAt(queue);
					TempQueue->Enqueue(cus);
					queue = QueueOfCustomer->MinTimeElementCTS();
				}
				else if (queue == nullptr) {
					Node* cus = this->ReturnRemove(table);
					TempQueue->Enqueue(cus);
					table = this->MinTimeElementCTS();
				}
				else if (table->time < queue->time) {
					Node* cus = this->ReturnRemove(table);
					TempQueue->Enqueue(cus);
					table = this->MinTimeElementCTS();
				}
				else {
					Node* cus = QueueOfCustomer->ReturnRemoveAt(queue);
					TempQueue->Enqueue(cus);
					queue = QueueOfCustomer->MinTimeElementCTS();
				}
			}

			// moi them vao 
			ToltalOfCTS = 0;
		}

		//IN RA 
		TempQueue->PrintReverse();
		delete TempQueue;


		// them tu hang doi vao ban 
		while (!QueueOfCustomer->isEmpty() && this->size < MAXSIZE) {
			Node* another = QueueOfCustomer->Dequeue();
			this->PushFromQueue(another);
		}
	}


	void LIGHT(int num)
	{
		/*cout << "-------------------" << endl;
		cout << "size of table " << this->size << endl;
		cout << "size of queue " << QueueOfCustomer->Size() << endl;*/

		if (num > 0) {
			if (pCurrent == nullptr) return;
			Node* p = pCurrent;
			do {
				p->data->print();
				p = p->next;
			} while (p != nullptr && p != pCurrent);
		}
		else if (num < 0) {
			if (pCurrent == nullptr) return;
			Node* p = pCurrent;
			do {
				p->data->print();
				p = p->prev;
			} while (p != nullptr && p != pCurrent);
		}
		else {
			QueueOfCustomer->Print();
		}


	}

	// HAM TEST
	//void AddClockWiseTEST(string name, int energy, Node* start) {
	//	Node* cus = new Node(name, energy, 0, nullptr, nullptr); 
	//	start->next->prev = cus;
	//	cus->next = start->next;
	//	cus->prev = start;
	//	start->next = cus;
	//	this->size++;
	//	pCurrent = start->next; // cap nhap lai vi tri X
	//}


	void run() {
		RED("ABC", 5);
		RED("AB", 2);
		RED("GOJO", -5);
		RED("SUKUNA", 2);
		RED("KIET", -4);
		RED("SIEUNHAN", 1);
		RED("LAM", 20);
		RED("mn", 10);
		RED("hs", -19);
		RED("VINH", -9);
		LIGHT(1);
		LIGHT(0);
		PURPLE();
		LIGHT(1);
		LIGHT(0);
		DOMAIN_EXPANSION();
		LIGHT(1);
		LIGHT(0);
	}
		
		/*RED("dy", 3);
		RED("pd", 7);
		AddClockWiseTEST("xe", 7, pCurrent);
		AddClockWiseTEST("lm", 2, pCurrent); 
		AddClockWiseTEST("ap", 7, pCurrent);
		AddClockWiseTEST("lr", 5, pCurrent); 
		AddClockWiseTEST("gw", -3, pCurrent); 
		AddClockWiseTEST("vr", -8, pCurrent); 
		AddClockWiseTEST("ti", -7, pCurrent);
		AddClockWiseTEST("kd", 9, pCurrent);
		AddClockWiseTEST("xc", 4, pCurrent);
		AddClockWiseTEST("hr", 5, pCurrent);
		AddClockWiseTEST("qv", -3, pCurrent);
		AddClockWiseTEST("xy", -6, pCurrent); 
		AddClockWiseTEST("nm", -2, pCurrent); 
		AddClockWiseTEST("ph", -3, pCurrent); 
		AddClockWiseTEST("lj", -8, pCurrent); 
		AddClockWiseTEST("jy", -7, pCurrent); 
		AddClockWiseTEST("qr", -5, pCurrent); 
		AddClockWiseTEST("hu", -6, pCurrent);
		AddClockWiseTEST("dk", -8, pCurrent);
		AddClockWiseTEST("bi", -8, pCurrent); 
		AddClockWiseTEST("vh", -7, pCurrent); 
		AddClockWiseTEST("ih", -2, pCurrent);
		AddClockWiseTEST("cf", -10, pCurrent); 
		pCurrent = pCurrent->next;
		LIGHT(1);
		cout << "......." << endl;
		UNLIMITED_VOID();*/

		//Queue* q = new Queue; 
		//q->Enqueue("A", 59, 0); 
		///*q->Enqueue("B", 20, 1);
		//q->Enqueue("C", 17, 2);
		//q->Enqueue("D", 13, 3);
		//q->Enqueue("F", 14, 4);
		//q->Enqueue("G", 23, 5);
		//q->Enqueue("H", 83, 6);
		//q->Enqueue("I", 36, 7);
		//q->Enqueue("J", 36, 8);
		//q->Enqueue("K", 11, 9);
		//q->Enqueue("L", 70, 10);
		//q->Enqueue("M", 65, 11);
		//q->Enqueue("N", 41, 12);
		//q->Enqueue("O", 42, 13);
		//q->Enqueue("P", 15, 14);*/


		//q->Print();
		//cout << q->ShellSort() << endl;
		//q->Print();
		
	//}
};