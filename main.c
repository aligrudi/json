/*
 * JSON - A SMALL PROGRAM TO WORK WITH JSON-ENCODED DATA
 *
 * Copyright (C) 2017-2024 Ali Gholami Rudi <ali at rudi dot ir>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "json.h"

static char *readall(int fd)
{
	char *dat = NULL;
	int dat_n = 0;
	int dat_sz = 0;
	int nr;
	while (1) {
		if (dat_n + 1 >= dat_sz) {
			dat_sz = dat_sz + (dat_sz ? dat_sz : 4096);
			if ((dat = realloc(dat, dat_sz)) == NULL)
				break;
		}
		if ((nr = read(fd, dat + dat_n, dat_sz - dat_n)) <= 0)
			break;
		dat_n += nr;
	}
	dat[dat_n] = '\0';
	return dat;
}

static int jprint(char *json, int nl)
{
	if (json != NULL && json[0] == '"')
		write(1, json + 1, json_len(json) - 2);
	if (json != NULL && json[0] != '"')
		write(1, json, json_len(json));
	write(1, nl ? "\n" : "\t", 1);
	return 0;
}

static int ccut(char **src, char *dst)
{
	int sep = 0;
	while (**src && **src != '.' && **src != ',')
		*dst++ = *(*src)++;
	*dst = '\0';
	if (**src)
		sep = *(*src)++;
	return sep;
}

static int follow(char *json, char *spec)
{
	char cur[256];
	char *new;
	int sep;
	json += json_ws(json);
	if (!spec[0])
		return jprint(json, 1);
	sep = ccut(&spec, cur);
	if (cur[0] == '*' && cur[1] == '\0') {
		char **data;
		if ((data = json_list(json)) != NULL) {
			int j;
			for (j = 0; data[j]; j++)
				follow(data[j], spec);
			free(data);
			return 0;
		}
		write(1, "\n", 1);
		return 1;
	}
	if (sep == ',') {
		jprint(json_dict_get(json, cur), 0);
		return follow(json, spec);
	}
	if ((new = json_dict_get(json, cur)) != NULL)
		return follow(new, spec);
	if ((new = json_list_get(json, atoi(cur))) != NULL)
		return follow(new, spec);
	write(1, "\n", 1);
	return 1;
}

int main(int argc, char *argv[])
{
	char *dat = readall(0);
	if (dat == NULL)
		return 1;
	follow(dat, argv[1]);
	free(dat);
	return 0;
}
