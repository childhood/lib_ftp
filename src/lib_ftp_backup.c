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


struct FtpFile
{
  const char *filename;
  FILE *stream;
};

 
static size_t
my_fwrite (void *buffer, size_t size, size_t nmemb, void *stream)
{	
  struct FtpFile *out = (struct FtpFile *) stream;

  if(out && !out->stream)
  {
    out->stream = fopen (out->filename, "wb");
    if (!out->stream)
     return -1;
  }
  return fwrite (buffer, size, nmemb, out->stream);
}


int ftp_backup (ftp_s *ftp, char *name_file, char *name_file_saved)
{

  CURL *curl;
  CURLcode res;

	struct FtpFile ftpfile = {name_file_saved, NULL};
 	
  /* Init */
	curl_global_init (CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init ();

  if(curl)
  {
    curl_easy_setopt (curl, CURLOPT_URL, g_strdup_printf ("http://%s/%s", ftp->server, name_file));
    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, my_fwrite);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, &ftpfile);
    curl_easy_setopt (curl, CURLOPT_VERBOSE, 0);

  	/* Use proxy */
    if (ftp->use_proxy == 1)
      curl_easy_setopt (curl, CURLOPT_PROXY, g_strdup_printf ("http://%s/%s/", ftp->proxy, ftp->proxy_port));

    res = curl_easy_perform (curl);

    /* Return in event of errors */
    if (CURLE_OK != res)
    {
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }

    curl_easy_cleanup (curl);
  }
 
  if (ftpfile.stream)
    fclose (ftpfile.stream);
 
  curl_global_cleanup ();
  return 0;
}
