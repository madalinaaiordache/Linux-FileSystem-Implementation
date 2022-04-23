// @Iordache Madalina Gabriela 323CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

// memory management
// auxiliary function to deallocate memory for all
// files in a directory
void clean_files(File** parent) {
    File *file = *parent;

    while (file != NULL) {
        if (file->name != NULL) 
            free(file->name);

        *parent = file->next;
        free(file);
        file = *parent;
    }
}

// function to deallocate memory for files/dirs in directories
void clean_memory(Dir **parent, int type) {
    Dir *dir = *parent;
    while (dir != NULL) {
        // deallocate memory for files
        clean_files(&(dir->head_children_files));
        dir->head_children_files = NULL;

        // recursively deallocate memory for directories
        clean_memory(&(dir->head_children_dirs), type);
        dir->head_children_dirs = NULL;
        
        free(dir->name);
        dir->name = NULL;

        if (type == 1)
            *parent = dir->next;
        else
            *parent = dir->head_children_dirs;

        free(dir);
        dir = NULL;
        dir = *parent;
    }
}

// function to create a new file with given name
void touch (Dir* parent, char* name) {
    File *file = parent->head_children_files;

    // take the length of the name
    int dim = strlen(name);

    // go through the list of files
    while (file != NULL) {
        // if we already found a file with the given name, error message
        if (strncmp(file->name, name, dim) == 0) {
            printf ("File already exists\n");
            return;
        }
        file = file->next;
    }
    file = parent->head_children_files;
    if (file == NULL) {
        // list of files is empty (directory has no files)
        // create and allocate memory for the new file
        parent->head_children_files = calloc(1, sizeof(File));
        parent->head_children_files->parent = parent;
        parent->head_children_files->next = NULL;
        parent->head_children_files->name = calloc(dim + 1, sizeof(char));
        parent->head_children_files->name[dim] = '\0';
        strncpy(parent->head_children_files->name, name, dim);
    } else {
        // there are files in the directory, add the new file at the end
        // create and allocate memory for the new file
        File *new_file = calloc(1, sizeof(File));
        new_file->parent = parent;
        new_file->next = NULL;
        new_file->name = calloc(dim + 1, sizeof(char));
        new_file->name[dim] = '\0';
        strncpy(new_file->name, name, dim);
        while (file->next != NULL) {
            file = file->next;
        }
        file->next = new_file;
    }
}

// function to create a new, empty directory with given name
void mkdir (Dir* parent, char* name) {
    Dir *dir = parent->head_children_dirs;

    // take the length of the name
    int dimens = strlen(name);

    // go through the list of directories
    while (dir != NULL) {
        // if we already found a dir with the given name, error message
        if (strncmp(dir->name, name, dimens) == 0) {
            printf ("Directory already exists\n");
            return;
        }
        dir = dir->next;
    }
    dir = parent->head_children_dirs;
    if (dir == NULL) {
        // list of dirs is empty (directory has no directories inside)
        // create and allocate memory for the new directory
        parent->head_children_dirs = calloc(1, sizeof(Dir));
        parent->head_children_dirs->parent = parent;
        parent->head_children_dirs->head_children_dirs = NULL;
        parent->head_children_dirs->head_children_files = NULL;
        parent->head_children_dirs->next = NULL;
        parent->head_children_dirs->name = calloc(dimens + 1, sizeof(char));
        parent->head_children_dirs->name[dimens] = '\0';
        strncpy(parent->head_children_dirs->name, name, dimens);
    } else {
        // there are other dirs in the directory, add the new dir at the end
        // create and allocate memory for the new directory
        Dir *new_dir = calloc(1, sizeof(Dir));
        new_dir->parent = parent;
        new_dir->head_children_dirs = NULL;
        new_dir->head_children_files = NULL;
        new_dir->name = calloc(dimens + 1, sizeof(char));
        new_dir->name[dimens] = '\0';
        new_dir->next = NULL;
        strncpy(new_dir->name, name, dimens);
        while (dir->next != NULL) {
            dir = dir->next;
        }
        dir->next = new_dir;
    }
}

// function to list computer files
void ls (Dir* parent) {
    // retain the first dir/file in the list of dirs/files
    File *file = parent->head_children_files;
    Dir *dir = parent->head_children_dirs;

    // go through the list of dirs and print all directories
    while (dir != NULL) {
        printf("%s\n", dir->name);
        dir = dir->next;
    }

    // go through the list of files and print all files
    while (file != NULL) {
        printf("%s\n", file->name);
        // move to the next file
        file = file->next;
    }
    
}

// function to remove a file from the current directory
void rm (Dir* parent, char* name) {
    File *file = parent->head_children_files;

    // take the length of the name
    int dim = strlen(name);

    if (file == NULL) {
        // if we couldn't find any file with the given name, eror message
        printf("Could not find the file\n");
        return;
    } else if (strncmp(file->name, name, dim) == 0) {
        // remove file at beginning of file list and deallocate memory
        parent->head_children_files = file->next;
        free(file->name);
        free(file);
    } else {
        // pointer to the file behind the file to be removed
        File *prev = NULL;

        // go through the list of files
        while (file != NULL) {
            // find the file to be removed
            if (strncmp(file->name, name, dim) == 0) {
                break;
            }
            prev = file;
            file = file->next;
        }
        if (file == NULL) {
            // if file with given name doesn't exist
            printf("Could not find the file\n");
            return;
        }

        // remove the file
        prev->next = file->next;

        // deallocate memory for the removed file
        free(file->name);
        free(file);
    }
}

// function to remove a directory
void rmdir (Dir* parent, char* name) {
    Dir *dir = parent->head_children_dirs;

    // take the length of the name
    int dim = strlen(name);

    if (dir == NULL) {
        // if we couldn't find any directory with the given name, eror message
        printf("Could not find the dir\n");
        return;
    } else if (strncmp(dir->name, name, dim) == 0) {
        // remove dir at beginning of directory list and deallocate memory
        parent->head_children_dirs = dir->next;
        clean_memory(&dir, 2);
    } else {
        // pointer to the dir behind the dir to be removed
        Dir *prev = NULL;

        // go through the list of directories
        while(dir != NULL) {
            // find the directory to be removed
            if (strncmp(dir->name, name, dim) == 0) {
                break;
            }
            prev = dir;
            dir = dir->next;
        }
        if (dir == NULL) {
            // if directory with given name doesn't exist
            printf("Could not find the dir\n");
            return;
        }
        // remove the directory
        prev->next = dir->next;

        // deallocate memory for the removed directory
        clean_memory(&dir, 2);
    }
}

// function to change current working directory
void cd(Dir** target, char *name) {
    // take the length of the name
    int dim = strlen(name);

    // move the directory back one directory(the parent directory)
    if (strncmp(name, "..", 2) == 0) {
        if ((*target)->parent != NULL)
            *target = (*target)->parent;
    } else {
        // implement cd command used to change the current working directory
        Dir *dir = (*target)->head_children_dirs;

        // go through the list of directories
        while (dir != NULL) {
            // find the directory we want to move to
            if (strncmp(dir->name, name, dim) == 0) {
                *target = dir;
                return;
            }
            dir = dir->next;
        }
        // if we couldn't find the directory we want to move to
        printf("No directories found!\n");
    }
}

// function to print the current working directory
char *pwd (Dir* target) {
    Dir *dir = target;

    // allocate memory for a string containing the path of current directory
    char *path = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));

    // concatenate the home directory to the path
    if (strncmp(dir->name, "home", 4) == 0) {
        strcat(path, "/home");
        return path;
    }

    // concatenate the parents of dir to the path
    while (dir->parent != NULL) {
        dir = dir->parent;
    }
    strcat(path, "/");
    strcat(path, dir->name);
    dir = dir->head_children_dirs;

    // concatenate the dir name to the path
    while (strcmp(dir->name, target->name) != 0) {
        strcat(path, "/");
        strcat(path, dir->name);
        dir = dir->head_children_dirs;
    }
    strcat(path, "/");
    strcat(path, dir->name);

    // find out what directory you are currently in
    return path;
}

// a recursive directory listing function that produces 
// a depth-indented listing of files
void tree (Dir* target, int level) {
    // pointers to the beginning of list of directories/files
    Dir *dir = target->head_children_dirs;
    File *file = target->head_children_files;

    // go through the list of directories
    while (dir != NULL) {
        // make the output take a graphical form on levels
        for (int i = 0; i < 4 * level; i++) {
            printf(" ");
        }
        // display the directories on levels recursively
        printf("%s\n", dir->name);
        tree(dir, level + 1);
        dir = dir->next;
    }
    while (file != NULL) {
        // make the output take a graphical form on levels
        for (int i = 0; i < 4 * level; i++) {
            printf(" ");
        }
        // display the files
        printf("%s\n", file->name);
        file = file->next;  
    }
}

// function to find given directory by name
Dir *get_directory(Dir* parent, char* name) {
    // store pointer to beginning of dir list
    Dir *dir = parent->head_children_dirs;

    // take length of the name
    int dim = strlen(name);

    // go through the list of directories
    while (dir != NULL) {
        // if the directory exists, then return it
        if(strncmp(dir->name, name, dim) == 0) {
            return dir;
        }
        dir = dir->next;
    }

    // if the directory doesn't exist, return NULL
    return NULL;
}

// function to find given file by name
File *get_file(Dir* parent, char* name) {
    // store pointer to beginning of file list
    File *file = parent->head_children_files;

    // take length of the name
    int dim = strlen(name);

    // go through the list of files
    while (file != NULL) {
        // if the file exists, then return it
        if(strncmp(file->name, name, dim) == 0)
            return file;
        file = file->next;
    }

    // if the file doesn't exist, return NULL
    return NULL;
}

// function to rename a directory or a file
void mv(Dir* parent, char *oldname, char *newname) {
    if (get_directory(parent, oldname) == NULL && 
        get_file(parent, oldname) == NULL) {
        // if the oldname of dir/file is nowhere to be found, error message
        printf("File/Director not found\n");
        return;
    } else if (get_directory(parent, newname) != NULL || 
               get_file(parent, newname) != NULL) {
        // if the newname of dir/file already exists, error message
        printf("File/Director already exists\n");
        return;
    } else if (get_directory(parent, oldname) != NULL ) {
        // store the directory whose name will be changed
        Dir *aux_dir = get_directory(parent, oldname);

        // store pointer at the beginning of dir list
        Dir *dir = parent->head_children_dirs;

        // go through the directory list
        while (dir->next != NULL) {
            dir = dir->next;
        }

        // create a directory having the newname
        mkdir(parent, newname);

        // remove the directory having the oldname
        rmdir(parent, aux_dir->name);

        return;
    } else if (get_file(parent, oldname) != NULL) {
        // store the file whose name will be changed
        File *aux_file = get_file(parent, oldname);

        // store pointer at the beginning of file list
        File *file = parent->head_children_files;

        // go through the file list
        while (file->next != NULL) {
            file = file->next;
        }

        // create a file having the newname
        touch(parent, newname);

        // remove the file having the oldname
        rm(parent, aux_file->name);

        return;
    }
}

void stop (Dir* target) {
    while (target->parent != NULL) {
        target = target->parent;
    }
    clean_memory(&target, 1);
}

// function to create home directory
Dir *create_home() {
    //create and allocate mameory for the home directory
    Dir *home = calloc(1, sizeof(Dir));

    home->parent = NULL;
    home->head_children_dirs = NULL;
    home->head_children_files = NULL;
    home->next = NULL;

    home->name = calloc(5, sizeof(char));
    strncpy(home->name, "home\0", 5);

    return home;
}

int main () {

	Dir *home = NULL;
    home = create_home();

    char *elem1 = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
    char *elem2 = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
    char *newname = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));;
	int level = 0;

	do
	{
		/*
		Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.
		*/

		scanf("%s", elem1);
        if (strncmp(elem1, "touch", 5) == 0) {
            scanf("%s", elem2);
            touch(home, elem2);
        } else if (strncmp(elem1, "mkdir", 5) == 0) {
			scanf("%s", elem2);
            mkdir(home, elem2);
        } else if (strncmp(elem1, "ls", 2) == 0) {
            ls(home);
        } else if (strncmp(elem1, "rmdir", 5) == 0) {
            scanf("%s", elem2);
            rmdir(home, elem2);
        } else if (strncmp(elem1, "rm", 2) == 0) {
            scanf("%s", elem2);
            rm(home, elem2);
        } else if (strncmp(elem1, "cd", 2) == 0) {
			scanf("%s", elem2);
            cd(&home, elem2);
        } else if (strncmp(elem1, "tree", 4) == 0) {
            scanf("%d", &level);
            tree(home, level);
        } else if (strncmp(elem1, "pwd", 3) == 0) {
            char *name = pwd(home);
            printf("%s\n", name);
            free(name);
        } else if (strncmp(elem1, "stop", 4) == 0) {
            stop(home);
            break;
        } else if (strncmp(elem1, "mv", 2) == 0) {
            scanf("%s", elem2);
			scanf("%s", newname);
            mv(home, elem2, newname);
        }
	} while (1);
	
    free(elem1);
    free(elem2);
    free(newname);

	return 0;
}
