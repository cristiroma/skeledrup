# What is this

This repository is useful for you if you want to a start a new Drupal project. Provides a directory layout, a project configuration file and a set of drush commands that helps you manage in the future the Drupal instance.

# How to use it

Suppose you want to bootstrap your new project based on this skeleton. So, what should you do?

1. Use _Download ZIP_ Github feature from right menu and unpack this release into a new directory. 
2. _(Optional)_ Configure drush: copy etc/drushrc.php to your ~/.drush/ directory 
3. Edit [etc/config.yml](etc/config.yml) to configure project global variables (used by everyone in the team)
4. Instruct each team member to override these global settings by creating a local *etc/local.yml* file
5. Customize [etc/project.yml](etc/project.yml) and [etc/profiles/standard/standard.info](etc/profiles/standard/standard.info) files to enable the modules you want to have for the new project
6. Run ``drush make etc/project.yml docroot`` to download the Drupal core & modules
7. Run ``drush site-install`` to create the database and install the Drupal instance on your computer

Now that you have a new project in place, you want to create a new repo from it and share with the team.

7. Use ``git init .`` to create a new repo for your project.
8. Commit & push the changes

# Writing tests

Please read [tests/readme.md](tests/readme.md) for more information on writing tests for your project.

# List of drush commands

## Project related

## Solr related commands
* ``drush reset-solr [solr-machine1,solr-machine2]`` - Show the configuration options for one or more blocks

## Customize drush built-in commands

* You override drush command options' configuring the $command_specific variable in [drush/drushrc.php](drush/drushrc.php). 
There are already is an example that overrides the ```drush site-install``` options.

## How do I ...

Q: ... add a new module into the project?

A: For a new project add the module to [etc/profiles/standard/standard.info](etc/profiles/standard/standard.info),
to [etc/project.yml](etc/project.yml), for existing projects also to ```docroot/profiles/standard/standard.info```. Then run ```drush build```.


# Credits

Credit to this project goes to:

* [Lullabot's boilerplate](https://github.com/Lullabot/drupal-boilerplate) and _also parts of the code have been copied into this project_.
* [drush-extras](https://www.drupal.org/project/drush_extras) - Useful extra commands available for drush

We're using it in our projects here @ [Eau de Web](http://www.eaudeweb.ro).


*Made in Romania*

