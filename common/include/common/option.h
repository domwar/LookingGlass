/*
KVMGFX Client - A KVM Client for VGA Passthrough
Copyright (C) 2017-2019 Geoffrey McRae <geoff@hostfission.com>
https://looking-glass.hostfission.com

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <stdbool.h>
#include "common/stringlist.h"

enum OptionType
{
  OPTION_TYPE_NONE = 0,
  OPTION_TYPE_INT,
  OPTION_TYPE_STRING,
  OPTION_TYPE_BOOL,
  OPTION_TYPE_CUSTOM
};

struct Option;

struct Option
{
  char * module;
  char * name;
  char * description;

  enum OptionType type;
  union
  {
    int    x_int;
    char * x_string;
    bool   x_bool;
    void * x_custom;
  }
  value;

  bool         (*parser   )(struct Option * opt, const char * str);
  bool         (*validator)(struct Option * opt, const char ** error);
  char       * (*toString )(struct Option * opt);
  StringList   (*getValues)(struct Option * opt);

  void    (*printHelp)();

  // internal use only
  bool failed_set;
};

// register an NULL terminated array of options
bool option_register(struct Option options[]);

// lookup the value of an option
struct Option * option_get       (const char * module, const char * name);
int             option_get_int   (const char * module, const char * name);
const char *    option_get_string(const char * module, const char * name);
bool            option_get_bool  (const char * module, const char * name);

// called by the main application to parse the command line arguments
bool option_parse(int argc, char * argv[]);

// called by the main application to load configuration from a file
bool option_load(const char * filename);

// called by the main application to validate the option values
bool option_validate();

// print out the options, help, and their current values
void option_print();

// final cleanup
void option_free();