/*
 * Copyright © 2011
 * Andrea Costa <nukemup@hotmail.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 * or write to the Free Software Foundation, Inc., 51 Franklin St
 * Fifth Floor, Boston, MA  02110-1301  USA
 */


#include "lib_ftp.h"


int
ftp_delete (ftp_s *ftp, char *file_to_delete)
{
  CURL *curl;
  CURLcode res;
  FILE *file;
  struct curl_slist *slist = NULL;

  /* Init */
  curl_global_init (CURL_GLOBAL_ALL);
  curl = curl_easy_init ();

  if (curl)
  {		
    curl_easy_setopt (curl,CURLOPT_URL, g_strdup_printf ("ftp://%s/", ftp->server));
    curl_easy_setopt (curl, CURLOPT_USERPWD, g_strdup_printf ("%s:%s", ftp->username, ftp->password));
    slist = curl_slist_append (slist, (char *) g_strdup_printf ("DELE %s", file_to_delete));
    curl_easy_setopt (curl, CURLOPT_QUOTE, slist);
    curl_easy_setopt (curl, CURLOPT_VERBOSE, 0);

  	/* Use proxy */
    if (ftp->use_proxy == 1)
      curl_easy_setopt (curl, CURLOPT_PROXY, g_strdup_printf ("http://%s/%s/", ftp->proxy, ftp->proxy_port));

    res = curl_easy_perform (curl);

    /* Return in event of errors */
    if (CURLE_OK != res)
    {
      curl_easy_cleanup (curl);
      curl_slist_free_all (slist);
      curl_global_cleanup ();
      return res;
    }
    curl_easy_cleanup (curl);
    curl_slist_free_all (slist);
  }

  curl_global_cleanup ();
  return 0;
}


