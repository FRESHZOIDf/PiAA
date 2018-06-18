#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#define MAXSIZE 5

using namespace std;

// ���������� ����
struct bohr_vertex
{
	int nextVertex[MAXSIZE];	 			// ������, ��� nextVertex[i] - ����� �������, � ������� �� ��������� �� ������� � ������� i � ��������
	bool isIt;								// ���, �����������, �-� �� ������� �������� �������
	int suffLink;							// ���������� ������
	int autoMove[MAXSIZE];					// ����������� �������� ��������
	int parent;								// ������� - ����
	char symbol;							// ������ �� ����� �� parent � ���� �������
	vector < int > patternNum;			// ����� ������-�������, ������������� �������� nextVertex[i]
	bohr_vertex() {}
	bohr_vertex(int parent, char symbol)
	{
		for (int i = 0; i < MAXSIZE; i++)
		{
			nextVertex[i] = -1;
		}
		isIt = false;
		suffLink = -1;
		for (int i = 0; i < MAXSIZE; i++)
		{
			autoMove[i] = -1;
		}
		this->parent = parent;
		this->symbol = symbol;
		patternNum.resize(0);
	}
};

vector < bohr_vertex > bohr;
vector < string > pattern;

// ������������� ����
void ini()
{
	bohr_vertex v(0, 0);
	bohr.push_back(v);
}


// ���������� ������� � ���
void add(const string & s)
{
	int n = 0;
	for (int i = 0; i < s.length(); i++)
	{
		int ch;
		switch (s[i])
		{
		case 'A':
			ch = 0;
			break;
		case 'C':
			ch = 1;
			break;
		case 'G':
			ch = 2;
			break;
		case 'T':
			ch = 3;
			break;
		case 'N':
			ch = 4;
			break;
		default:
			ch = 5;
			break;
		}
		if (bohr[n].nextVertex[ch] == -1)	// ��� �����
		{
			bohr_vertex v(n, ch);
			bohr.push_back(v);
			bohr[n].nextVertex[ch] = bohr.size() - 1;
		}
		n = bohr[n].nextVertex[ch];
	}
	bohr[n].isIt = true;
	pattern.push_back(s);
	bohr[n].patternNum.push_back(pattern.size() - 1);

}

int getAutoMove(int v, int ch);

// �������� ���������� ������
int getSuff(int v)
{
	if (bohr[v].suffLink == -1)
		if (v == 0 || bohr[v].parent == 0)
			bohr[v].suffLink = 0;
		else
			bohr[v].suffLink = getAutoMove(getSuff(bohr[v].parent), bohr[v].symbol);
	return bohr[v].suffLink;
}

int getAutoMove(int v, int ch)
{
	if (bohr[v].autoMove[ch] == -1)
		if (bohr[v].nextVertex[ch] != -1)
			bohr[v].autoMove[ch] = bohr[v].nextVertex[ch];
		else
			if (v == 0)
				bohr[v].autoMove[ch] = 0;
			else
				bohr[v].autoMove[ch] = getAutoMove(getSuff(v), ch);
	return bohr[v].autoMove[ch];
}


void out(int v, int i, vector < int > & cnt, const vector < int > & len)
{
	for (int u = v; u != 0; u = getSuff(u))
		if (bohr[u].isIt)
		{
			//cout << i - pattern[bohr[u].patternNum].length() + 1 << " " << bohr[u].patternNum + 1 << endl;
			for (const auto &j : bohr[u].patternNum)
				if (i - len[j] < cnt.size()) cnt[i - len[j]]++;
		}
}

vector < int > patterns(stringstream& string_pattern, char joker)
{
	vector < int > len;
	int len_ = 0;
	string buf;
	while (getline(string_pattern, buf, joker))
	{
		if (buf.size() > 0)
		{
			len_ += buf.size();
			len.push_back(len_);
			add(buf);
		}
		len_++;
	}
	return len;
}

void out_2(const vector<int>& cnt, int t_size)
{
	for (int i = 0; i < t_size; i++)
		if (cnt[i] == pattern.size())
			cout << i + 1 << endl;
}

void find(const string &s, vector < int > & cnt, const vector < int > & len)
{
	int u = 0;
	for (int i = 0; i < s.length(); i++)
	{
		int ch;
		switch (s[i])
		{
		case 'A':
			ch = 0;
			break;
		case 'C':
			ch = 1;
			break;
		case 'G':
			ch = 2;
			break;
		case 'T':
			ch = 3;
			break;
		case 'N':
			ch = 4;
			break;
		default:
			ch = 5;
			break;
		}
		u = getAutoMove(u, ch);
		out(u, i + 1, cnt, len);
	}
}

int main()
{
	ini();
	int n;
	string text, keyword;
	char j;
	cin >> text;
	cin >> keyword;
	cin >> j;
	stringstream str_stream(keyword);
	vector < int > cnt(text.size(), 0);
	vector < int > len = patterns(str_stream, j);
	find(text, cnt, len);
	out_2(cnt, text.size());
}