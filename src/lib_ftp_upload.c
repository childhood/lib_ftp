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


static size_t
read_callback (void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t retcode = fread(ptr, size, nmemb, stream);
  return retcode;
}

 
int
ftp_upload (ftp_s *ftp)
{
  CURL *curl;
  CURLcode res;
  FILE *hd_src;
  curl_off_t fsize;
  struct stat file_info;
	
  /* Init */
  curl_global_init (CURL_GLOBAL_ALL);
  curl = curl_easy_init ();
 
  if (curl)
  {
	  if (stat (ftp->path_file_upload, &file_info))
    {
      printf ("* Error - Couldnt open '%s': %s\n", ftp->path_file_upload, strerror (errno));
      return -1;
    }
    fsize = (curl_off_t) file_info.st_size;
	  hd_src = fopen (ftp->path_file_upload, "rb");

    curl_easy_setopt (curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt (curl,CURLOPT_URL, (char *) g_strdup_printf ("ftp://%s/%s", ftp->server, ftp->name_file_upload));
    curl_easy_setopt (curl, CURLOPT_USERPWD, g_strdup_printf ("%s:%s", ftp->username, ftp->password));
    curl_easy_setopt (curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt (curl, CURLOPT_READDATA, hd_src);
    curl_easy_setopt (curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t) fsize);

  	/* Use proxy */
    if (ftp->use_proxy == 1)
      curl_easy_setopt (curl, CURLOPT_PROXY, g_strdup_printf ("http://%s/%s/", ftp->proxy, ftp->proxy_port));

    res = curl_easy_perform (curl);

    /* Return in event of errors */
    if (CURLE_OK != res)
    {
      fclose (hd_src);
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }

    curl_easy_cleanup (curl);
  }

  fclose (hd_src);

  curl_global_cleanup ();
  return 0;
}


int
ftp_upload_set_files (ftp_s *ftp, char *path_file, char *name_file)
{
  CURL *curl;
  CURLcode res;
  FILE *hd_src;
  curl_off_t fsize;
  struct stat file_info;
	
  /* Init */
  curl_global_init (CURL_GLOBAL_ALL);
  curl = curl_easy_init ();
 
  if (curl)
  {
	  if (stat (path_file, &file_info))
    {
      printf ("* Error - Couldnt open '%s': %s\n", path_file, strerror (errno));
      return -1;
    }
    fsize = (curl_off_t) file_info.st_size;
	  hd_src = fopen (path_file, "rb");

    curl_easy_setopt (curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt (curl,CURLOPT_URL, (char *) g_strdup_printf ("ftp://%s/%s", ftp->server, name_file));
    curl_easy_setopt (curl, CURLOPT_USERPWD, g_strdup_printf ("%s:%s", ftp->username, ftp->password));
    curl_easy_setopt (curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt (curl, CURLOPT_READDATA, hd_src);
    curl_easy_setopt (curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t) fsize);

  	/* Use proxy */
    if (ftp->use_proxy == 1)
      curl_easy_setopt (curl, CURLOPT_PROXY, g_strdup_printf ("http://%s/%s/", ftp->proxy, ftp->proxy_port));

    res = curl_easy_perform (curl);

    /* Return in event of errors */
    if (CURLE_OK != res)
    {
      fclose (hd_src);
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }

    curl_easy_cleanup (curl);
  }

  fclose (hd_src);

  curl_global_cleanup ();
  return 0;
}
