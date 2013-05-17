#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 30

#define IN_START_TAG 0
#define IN_END_TAG 10
#define IN_TAG_BODY 20
#define DEFAULT_STATE 30
#define IN_GENERIC_TAG 40

FILE *my_file = NULL;

int state = DEFAULT_STATE;
int has_correct_key = 0;
int in_good_tag = 0;

char desired_key[] = "SENS0710:23326:IntSolIrr";
char desired_tag[BUF_SIZE] = "Mean";
char desired_data[BUF_SIZE];
char current_tag[BUF_SIZE];

void state_message()
{
	printf("Changed to ");
	switch (state) {
	case DEFAULT_STATE:
		printf("the default state");
		break;
	case IN_START_TAG:
		printf("start tag mode");
		break;
	case IN_END_TAG:
		printf("end tag mode");
		break;
	case IN_GENERIC_TAG:
		printf("generic tag mode");
		break;
	case IN_TAG_BODY:
		printf("tag body mode");
		break;
	default:
		printf("NULLL BADD");
		break;
	}
	printf("\n");
}

void change_state(int new_state)
{
	state = new_state;
	#ifdef _DEBUG
	state_message();
	#endif
}

void process_start_tag(char c)
{
	switch (c) {
	case '>':
		#ifdef _DEBUG
		printf("Just read start tag: %s\n", current_tag);
		#endif
		if (strcmp(current_tag, desired_tag) == 0) {
			#ifdef _DEBUG
			printf("FOUND A TAG MATCH\n");
			#endif
			in_good_tag = 1;
		}
		current_tag[0] = '\0';
		change_state(DEFAULT_STATE);
		break;
	default:
		/* Add the character to the current tag name */
		if (strlen(current_tag) < BUF_SIZE) {
			strncat(current_tag, &c, 1);
		} else {
			#ifdef _DEBUG
			printf("Buffer overflow!\n");
			#endif
		}
	}
}

void process_end_tag(char c)
{
	switch (c) {
	case '>':
		change_state(DEFAULT_STATE);
		break;
	default:
		break;
	}
}

void process_tag_body(char c)
{
	switch (c) {
	case '<':
		if (in_good_tag) {
			in_good_tag = 0;
			printf("Data: %s\n", desired_data);
			desired_data[0] = '\0';
		}
		change_state(IN_END_TAG);
		break;
	default:
		if (in_good_tag) {
			#ifdef _DEBUG
			printf("Adding data!\n");
			#endif
			strncat(desired_data, &c, 1);
		} else {
			break;
		}
	}
}

void process_generic_tag(char c)
{
	if (c == '<') {
		return;
	}
	if (c == '/') {
		change_state(IN_END_TAG);
	} else {
		change_state(IN_START_TAG);
		process_start_tag(c);
	}
}

void process_default(char c)
{
	switch (c) {
	case '\n':
		break;
	case '<':
		change_state(IN_GENERIC_TAG);
		break;
	default:
		change_state(IN_TAG_BODY);
		process_tag_body(c);
	}
}

int main(int argc, char **argv)
{
	if (argc == 1) {
		printf("Please provide a file for processing\n");
		return 1;
	}
	char * filename = argv[1];
	printf("-- %s --\n", filename);
	my_file = fopen(filename, "r");
	if (my_file == NULL) {
		printf("File not found\n");
		return 1;
	}

	char current;
	while ((current = fgetc(my_file)) != EOF) {
		switch (state) {
			case IN_START_TAG:
				process_start_tag(current);
				break;
			case IN_END_TAG:
				process_end_tag(current);
				break;
			case IN_TAG_BODY:
				process_tag_body(current);
				break;
			case IN_GENERIC_TAG:
				process_generic_tag(current);
				break;
			case DEFAULT_STATE:
			default:
				process_default(current);
		}
	}
	fclose(my_file);
}
