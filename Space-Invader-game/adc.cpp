// SpiceGen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "stdio.h"
#include "conio.h"

FILE *fpI, *fpO;
const int MaxName = 20;
const int Max = 50;
const int MaxVal = 500;
const int DW = 32;

struct Signal{
	char Name[MaxName];
	int  Width;
	int  Numbers;
	char  value[MaxVal][DW];
	int  Delay[MaxVal];
} Wires[Max];
char Word[MaxName];
char Initial[10] = "initial^", Begin[10] = "begin^", End[5] = "end^";
int  WireCount, Time, Delay;

int FindaWord()
{
	char C;
	int i = 0;
	fscanf(fpI, "%c", &C);
	while (!((C <= 'z' && C >= 'a') || (C <= 'Z' && C >= 'A') || (C <= '9' && C >= '0') || C == '_' || C == '#') && !feof(fpI))
		fscanf(fpI, "%c", &C);
	if (feof(fpI))
		return 1000;
	while ((C <= 'z' && C >= 'a') || (C <= 'Z' && C >= 'A') || (C <= '9' && C >= '0') || C == '_' || C == '#')
	{
		Word[i] = C;
		i++;
		fscanf(fpI, "%c", &C);
	}
	Word[i] = '\0';
	return i;
}

int CompareWord(char c)
{
	int OK = 1, Len;
	char Temp[10];
	if (c == 'i')
	{
		Temp[0] = 'i'; Temp[1] = 'n'; Temp[2] = 'i'; Temp[3] = 't'; Temp[4] = 'i'; Temp[5] = 'a'; Temp[6] = 'l'; Temp[7] = '^'; Len = 7;
	}
	else if (c == 'b')
	{
		Temp[0] = 'b'; Temp[1] = 'e'; Temp[2] = 'g'; Temp[3] = 'i'; Temp[4] = 'n'; Temp[5] = '^'; Len = 5;
	}
	else if (c == 'e')
	{
		Temp[0] = 'e'; Temp[1] = 'n'; Temp[2] = 'd'; Temp[3] = '^'; Len = 3;
	}
	for (int j = 0; j < Len;j++)
		if (Word[j] != Temp[j])
			return 0;
		return 1;
}

int FindIndex(int Length)
{
	int i, j;
	for (i = 0; i < Max; i++)
	{
		for (j = 0; j < Length; j++)
		{
			if (Word[j] != Wires[i].Name[j])
				break;
		}
		if (j >= Length)
			return i;
	}
	return 1000;
}

void ReadaLine()
{
	char g, Line[200];
	int Index, Length, i, j, TempW, TempV, Mode, Value[DW];
	unsigned int W;
	Length = FindaWord();
	if (Length == 1000)
		return;
	g = 'i';
	Delay = 0;
	if (CompareWord(g) == 1)
	{
		Time = 0;
		return;
	}
	g = 'b';
	if (CompareWord(g) == 1)
		return;
	g = 'e';
	if (CompareWord(g) == 1)
		return;
	if (Word[0] == '#')
	{
		Delay = 0;
		i = 1;
		while (Word[i] <= '9' && Word[i] >= '0')
		{
			Delay = Delay * 10 + (Word[i] - 48);
			i++;
		}
		Length = FindaWord();
		if (Length == 1000)
			return;
	}
	W = 0;
	fscanf(fpI, "%c", &g);
	while (g <= '0' || g >= '9')
		fscanf(fpI, "%c", &g);
	TempW = g - 48;
	fscanf(fpI, "%c", &g);
	while (g >= '0' && g <= '9')
	{
		TempW *= 10;
		TempW += (g - 48);
		fscanf(fpI, "%c", &g);
	}
	fscanf(fpI, "%c", &g);
	if (g == 'b')		Mode = 0;//binary
	else if (g == 'd')	Mode = 1;//decimal
	else if (g == 'o')	Mode = 2;//octal
	else				Mode = 3;//Hexadecimal
	fscanf(fpI, "%c", &g);
	i = 0;
	while (g != ';')
	{
		Line[i] = g;
		i++;
		fscanf(fpI, "%c", &g);
	}
	TempV = 0;
	if (Mode == 0)			//Binary
	{
		if (Word[0] == 'D')
			TempV = 0;
		for (j = 0; j < i; j++)
		{
			Value[j] = Line[j] - 48;
		}
	}
	else if (Mode == 1)		//Decimal
	{
		for (j = 0; j < i; j++)
		{
			TempV *= 10;
			TempV += (Line[j] - 48);
		}
		W = 1 << 31;
		for (j = 0; j < TempW; j++)
		{
			Value[j] = (TempV >= W) ? 1 : 0;
			TempV = TempV % W;
			W = W >> 1;
		}
	}
	else if (Mode == 2)		//Octal
	{
		for (j = 0; j < i; j++)
		{
			TempV *= 8;
			TempV += (Line[j] - 48);
		}
		W = 1 << 31;
		for (j = 0; j < TempW; j++)
		{
			Value[j] = (TempV >= W) ? 1 : 0;
			TempV = TempV % W;
			W = W >> 1;
		}
	}
	else if (Mode == 3)		//Hexadecimal
	{
		for (j = 0; j < i; j++)
		{
			TempV *= 16;
			if (Line[j] >= '0' && Line[j] <= '9')
				TempV += (Line[j] - 48);
			else
			{
				if (Line[j] == 'a' || Line[j] == 'A')
					TempV += 10;
				else if (Line[j] == 'b' || Line[j] == 'B')
					TempV += 11;
				else if (Line[j] == 'c' || Line[j] == 'C')
					TempV += 12;
				else if (Line[j] == 'd' || Line[j] == 'D')
					TempV += 13;
				else if (Line[j] == 'e' || Line[j] == 'E')
					TempV += 14;
				else if (Line[j] == 'f' || Line[j] == 'F')
					TempV += 15;
			}
		}
		W = 1 << 31;
		for (j = 0; j < TempW; j++)
		{
			/*Value[j] = (TempV >= W) ? 1 : 0;
			if (W > 0)
			{
				TempV = TempV % W;
				W = W >> 1;
			}*/
			Value[j] = (TempV >= W) ? 1 : 0;
			TempV = TempV % W;
			W = W >> 1;
		}
	}
	Index = FindIndex(Length);
	Time += Delay;
	if (Index == 1000)
	{
		for (i = 0; i < Length; i++)
			Wires[WireCount].Name[i] = Word[i];
		Wires[WireCount].Name[i] = '^';
		Wires[WireCount].Width = TempW;
		Wires[WireCount].Numbers = 1;
		for (j = 0; j < TempW; j++)
		{
			Wires[WireCount].value[0][TempW - 1 - j] = (Value[j] == 1) ? '1' : '0';
		}
		Wires[WireCount].Delay[0] = Time;
		for (i = 0; i < Length; i++)
			printf("%c", Wires[WireCount].Name[i]);
		if (Wires[WireCount].Name[0] == 'D')
			i = 0;
		WireCount++;
	}
	else
	{
		for (j = 0; j < TempW; j++)
		{
			Wires[Index].value[Wires[Index].Numbers][TempW - 1 - j] = (Value[j] == 1) ? '1' : '0';
		}
		Wires[Index].Delay[Wires[Index].Numbers] = Time;
		Wires[Index].Numbers++;
		for (i = 0; i < Length; i++)
			printf("%c", Wires[Index].Name[i]);
		if (Wires[Index].Name[0] == 'D')
			i = 0;
	}
	while (g != '\n')// || !feof(fpI))
		fscanf(fpI, "%c", &g);
	printf("\n");
}

void ParseFile()
{
	while (!feof(fpI))
		ReadaLine();
}

void WriteOutput()
{
	int i, j, k, l, m, W, Fin = 0, Temp;
	for (i = 0; i < WireCount; i++)
	{
		W = Wires[i].Width;
		for (j = 0; j < W; j++)
		{
			fprintf(fpO, "V_");
			k = 0;
			while (Wires[i].Name[k] != '^')
			{
				fprintf(fpO, "%c", Wires[i].Name[k]);
				k++;
			}
			l = j;
			if (W >= 100)
			{
				fprintf(fpO, "%d", l / 100);
				if (l >= 100) l = l % 100;
			}
			if (W >= 10)
			{
				fprintf(fpO, "%d", l / 10);
				if (l >= 10) l = l % 10;
			}
			if (W > 1)
			{
				fprintf(fpO, "%d\t", l);
			}
			else
				fprintf(fpO, "\t");
			k = 0;
			l = j;
			while (Wires[i].Name[k] != '^')
			{
				fprintf(fpO, "%c", Wires[i].Name[k]);
				k++;
			}
			if (W >= 100)
			{
				fprintf(fpO, "%d", l / 100);
				if (l >= 100) l = l % 100;
			}
			if (W >= 10)
			{
				fprintf(fpO, "%d", l / 10);
				if (l >= 10) l = l % 10;
			}
			if (W > 1)
			{
				fprintf(fpO, "%d\t0 PWL ", l);
			}
			else
				fprintf(fpO, "\t0 PWL ");
			if (Wires[i].Name[0] == 'D' && j == 0)
				j += 0;
			k = 0;
			for (l = 0; l < Wires[i].Numbers; l++)
			{
				if (l == 0)
				{
					fprintf(fpO, "%dn ", Wires[i].Delay[l]);
					if (Wires[i].value[l][j] == '0')
						fprintf(fpO, "0v");
					else
						fprintf(fpO, "vddl");
				}
				else if (Wires[i].value[l][j] != Wires[i].value[l-1][j])
				{
					fprintf(fpO, ", %d.95n ", Wires[i].Delay[l] - 1);
					if (Wires[i].value[l - 1][j] == '0')
						fprintf(fpO, "0v, %dn vddl", Wires[i].Delay[l]);
					else
						fprintf(fpO, "vddl, %dn 0v", Wires[i].Delay[l]);
					k++;
					Temp = Wires[i].value[l][j];
					m = l;
					while (m < Wires[i].Numbers)
					{
						if (Wires[i].value[m][j] != Temp)
						{
							Fin = 1;
							break;
						}
						m++;
					}
				}
				if (Fin == 1 && k == 3 && l != Wires[i].Numbers - 1)
				{
					fprintf(fpO, "\n+							");
					k = 0;
				}
				Fin = 0;
			}
			fprintf(fpO, "\n");
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	fpI = fopen("Input.v", "r");
	fpO = fopen("Output.txt", "w");
	Time = 0;
	WireCount = 0;
	ParseFile();
	WriteOutput();
	return 0;
}
