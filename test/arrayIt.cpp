#include <iostream>
#include <vector>
using namespace std;

int main()
{
	vector<vector<int>> a(4, vector<int>(4, 0));
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << a[i][j] << " ";
		cout << endl;
	}
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			a[i][j] = 4 * i + j;

	cout << "\n";

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << a[i][j] << " ";
		cout << endl;
	}
	vector<int>::iterator d = a[0].begin();

	for (int i = 0; i<16; i++, d++, cin>> *new int)
		cout << *d << " " << (*new int = 3);

	return 0;
}