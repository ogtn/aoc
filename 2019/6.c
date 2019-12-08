#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int MAX_OBJETCS = 2048;

typedef struct object
{
	char name[4];
	char parent[4];
	int children[4];
	int nb_orbits;
} object;

int hash(char c)
{
	if(c >= 'A' && c <= 'Z')
		return c - 'A' + 10;

	return c - '0';
}

int main(void)
{
	static object objects[MAX_OBJETCS];
	static short objects_map[36][36][36];
	char buff[10];
	short nb = 1;

	memcpy(objects[0].name, "COM", 3);

	while(fgets(buff, 10, stdin))
	{
		memcpy(objects[nb].parent, buff, 3);
		memcpy(objects[nb].name, buff + 4, 3);
		objects_map
			[hash(buff[4])]
			[hash(buff[5])]
			[hash(buff[6])] = nb;
		nb++;
	}

	for(int i = 1; i < nb; i++)
	{
		short id = i;
		short id_parent;

		do
		{
			objects[id].nb_orbits++;
			id_parent = objects_map
				[hash(objects[id].parent[0])]
				[hash(objects[id].parent[1])]
				[hash(objects[id].parent[2])];
				
			id = id_parent;
		} while(id_parent);
	}

	int res = 0;

	for(int i = 1; i < nb; i++)
		res += objects[i].nb_orbits;

	printf("nb_orbits for COM: %d\n", res);

	return EXIT_SUCCESS;
}
