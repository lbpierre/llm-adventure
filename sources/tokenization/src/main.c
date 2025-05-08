#include "array_utils.h"
#include "reader.h"
#include "set_utils.h"
#include "tokenizer.h"
#include "uthash.h"
#include <pcre2.h>
#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char *argv[])
{

  if (argc < 2)
    {
      fprintf (stderr, "Usage: %s <file_path>\n", argv[0]);
      return EXIT_FAILURE;
    }

  const char *filepath = argv[1];
  PCRE2_SPTR pattern = (PCRE2_SPTR) "--|[.;_,?!'\"()]|\\s";
  Tokenizer *tokenizer = create_tokenizer (filepath, pattern);

  const char *text = "It's the last he painted, you know,\n"
                     "Mrs. Gisburn said with pardonable pride. And this--one \"escaped quote ok\" "
                     "(this is it) XXXXXXXXXX ok, Hello there!";
  printf ("encode the following text with the tokenizer:\n");
  printf ("%s\n ------ \n", text);

  Array *encoded = tokenizer->encode (tokenizer, text);

  printf ("Tokenized input\n");
  array_print (encoded);

  printf ("Decoded back\n");
  char *decoded = tokenizer->decode (tokenizer, encoded);
  printf ("%s\n", decoded);
  array_free (encoded);
  free_tokenizer (tokenizer);

  return 0;
}
