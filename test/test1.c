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


#include "../src/lib_ftp.h"
#include <stdlib.h>


int main ()
{
  int ret;

  /* Allocate app's struct */
  ftp_s *ftp;
  ftp = malloc (1 * sizeof (ftp_s));

  /* Define settings */
  ftp->server           = "server"; /* es: "oneandy.altervista.org" */
  ftp->username         = "username";
  ftp->password         = "password";
  ftp->path_file_upload = "data.txt";
  ftp->name_file_upload = "data_uploaded.txt";
  ftp->use_proxy        = 0;
  ftp->proxy            = NULL;
  ftp->proxy_port       = NULL;

  printf ("* explore server...\n");

  /*
    EXPLORE SERVER saving the content in "data.txt"
    No iterative protocol - It doesn't explore the sub-directories
  */
  ret = ftp_explore (ftp, "data.txt");

  printf ("* start backup...\n");

  /*
    BACKUP FILE
    You can download the file and save it with another name.
    No iterative prototocol - One file at the time
  */
  ret = ftp_backup (ftp, "file.pdf", "saved_file.pdf");

  printf ("* start upload...\n");

  /*
    UPLOAD FILE
    No iterative prototocol - One file at the time
  */
  /*
  ret = ftp_upload (ftp);
  */
  /* or */
  ret = ftp_upload_set_files (ftp, "data.txt", "uploaded_data.txt");


  /*
    DELETE FILE
    No iterative prototocol - One file at the time
  */
  ret = ftp_delete (ftp, "uploaded_data.txt");


  return 0;
}
