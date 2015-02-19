# Security checklist measures

# 1. Apache web server security configuration

## Virtual host configuration

```
    # Disable .htaccess in sites/default/files! Deny arbitrary script execution in subdirectories hack.
    # Hackers create a subdir/.htaccess with "php_engine on" and can still execute arbitrary code!
    <Directory /absolute/path/to/docroot/sites/default/files>
        AllowOverride None
        # Turn off all options we don't need.
        Options None
        Options +FollowSymLinks

        # Set the catch-all handler to prevent scripts from being executed.
        SetHandler Drupal_Security_Do_Not_Remove_See_SA_2006_006
        <Files *>
          # Override the handler again if we're run later in the evaluation list.
          SetHandler Drupal_Security_Do_Not_Remove_See_SA_2013_003
        </Files>

        # If we know how to do it safely, disable the PHP engine entirely.
        <IfModule mod_php5.c>
          php_flag engine off
        </IfModule>
    </Directory>

    # Disable .htaccess in Apache temp directory for this intance to prevent arbitrary code execution in subdirectories!
    <Directory /absolute/path/to/private>
        AllowOverride None
        Deny from all
        # Turn off all options we don't need.
        Options None
        Options +FollowSymLinks
        # Set the catch-all handler to prevent scripts from being executed.
        SetHandler Drupal_Security_Do_Not_Remove_See_SA_2006_006
        <Files *>
            # Override the handler again if we're run later in the evaluation list.
            SetHandler Drupal_Security_Do_Not_Remove_See_SA_2013_003
        </Files>
        # If we know how to do it safely, disable the PHP engine entirely.
        <IfModule mod_php5.c>
            php_flag engine off
        </IfModule>
   </Directory>

    # Disable .htaccess in Apache temp directory for this intance to prevent arbitrary code execution in subdirectories!
    <Directory /absolute/path/to/temp>
        AllowOverride None
        Deny from all
        # Turn off all options we don't need.
        Options None
        Options +FollowSymLinks
        # Set the catch-all handler to prevent scripts from being executed.
        SetHandler Drupal_Security_Do_Not_Remove_See_SA_2006_006
        <Files *>
            # Override the handler again if we're run later in the evaluation list.
            SetHandler Drupal_Security_Do_Not_Remove_See_SA_2013_003
        </Files>
        # If we know how to do it safely, disable the PHP engine entirely.
        <IfModule mod_php5.c>
            php_flag engine off
        </IfModule>
   </Directory>
```


## docroot/

This directory must be read-only to apache (and all files inside except *sites/default/files*)!

```
drwxr-x---. php apache unconfined_u:object_r:httpd_sys_content_t:s0 .
drwxr-x---. php apache unconfined_u:object_r:httpd_sys_content_t:s0 ..
-rw-r-----. php apache unconfined_u:object_r:httpd_sys_content_t:s0 authorize.php
-rw-r-----. php apache unconfined_u:object_r:httpd_sys_content_t:s0 cron.php
-rw-r-----. php apache unconfined_u:object_r:httpd_sys_content_t:s0 favicon.ico
-rw-r-----. php apache unconfined_u:object_r:httpd_sys_content_t:s0 .htaccess
drwxr-x---. php apache unconfined_u:object_r:httpd_sys_content_t:s0 includes
-rw-r-----. php apache unconfined_u:object_r:httpd_sys_content_t:s0 index.php
-rw-r-----. php apache unconfined_u:object_r:httpd_sys_content_t:s0 install.php
drwxr-x---. php apache unconfined_u:object_r:httpd_sys_content_t:s0 misc
drwxr-x---. php apache unconfined_u:object_r:httpd_sys_content_t:s0 modules
drwxr-x---. php apache unconfined_u:object_r:httpd_sys_content_t:s0 profiles
-rw-r-----. php apache unconfined_u:object_r:httpd_sys_content_t:s0 robots.txt
drwxr-x---. php apache unconfined_u:object_r:httpd_sys_content_t:s0 scripts
drwxr-x---. php apache unconfined_u:object_r:httpd_sys_content_t:s0 sites
drwxr-x---. php apache unconfined_u:object_r:httpd_sys_content_t:s0 themes
-rw-r-----. php apache unconfined_u:object_r:httpd_sys_content_t:s0 update.php
-rw-r-----. php apache unconfined_u:object_r:httpd_sys_content_t:s0 xmlrpc.php
```

# Filesystem security configuration

### docroot/sites/default/files

(Optional) Use git to track changes in the files directory to easily find out suspect files (since last commit)

  * git init . && git add . && git commit -m "Initial revision"
  * .git directory should be owned by root account in order to prevent commits to the repo by apache user.

Typical setup for files directory

```
drwxrwx---. apache apache unconfined_u:object_r:httpd_sys_rw_content_t:s0 .
drwxr-x---. php    apache unconfined_u:object_r:httpd_sys_content_t:s0 ..
-rw-r-----. php apache unconfined_u:object_r:httpd_sys_rw_content_t:s0 .htaccess
drwxrwx---. apache apache unconfined_u:object_r:httpd_sys_rw_content_t:s0 css
drwxrwx---. apache apache unconfined_u:object_r:httpd_sys_rw_content_t:s0 ctools
drwxrwx---. apache apache unconfined_u:object_r:httpd_sys_rw_content_t:s0 js
drwxrwx---. apache apache unconfined_u:object_r:httpd_sys_rw_content_t:s0 styles
```

### temp/ files directory

Typical permission configuration for temporary directory

```
drwxrwx---. apache apache unconfined_u:object_r:httpd_sys_rw_content_t:s0 .
drwxr-x---. php apache unconfined_u:object_r:httpd_sys_content_t:s0    ..
drwxrwx---. apache apache unconfined_u:object_r:httpd_sys_rw_content_t:s0 temporary-uploaded-fragment
-rw-r-----. root   apache unconfined_u:object_r:httpd_sys_content_t:s0 .htaccess
```


### private/ files directory

Typical permission configuration for private files directory

```
drwxrwx---. apache apache unconfined_u:object_r:httpd_sys_rw_content_t:s0 .
drwxr-x---. php    apache unconfined_u:object_r:httpd_sys_content_t:s0 ..
drwxrwx---. apache apache unconfined_u:object_r:httpd_sys_rw_content_t:s0 decision
-rw-r-----. root   apache unconfined_u:object_r:httpd_sys_content_t:s0 .htaccess
```

# Drupal instance security configuration checklist

1. Always set explicit temporary directory outside /tmp and configure security properly as stated above

2. Disable PHP input filter and all modules that allows entering PHP from the back-end

