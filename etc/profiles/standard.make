; The Drush Make API version. This should always be 2.
api = 2

; The version of Drupal the profile is built for. Although you can leave this
; as '7.x', it's better to be precise and define the exact version of core your
; distribution works with.
core = 7.34

projects[drupal][type] = core

; Very common modules used in almost all projects
projects[ctools][subdir] = contrib
projects[date][subdir] = contrib
projects[entity][subdir] = contrib
projects[entityreference][subdir] = contrib
projects[features][subdir] = contrib
projects[ftools][subdir] = contrib
projects[wysiwyg][subdir] = contrib
projects[jquery_update][subdir] = contrib
projects[libraries][subdir] = contrib
projects[link][subdir] = contrib
projects[pathauto][subdir] = contrib
projects[strongarm][subdir] = contrib
projects[taxonomy_access_fix][subdir] = contrib
projects[token][subdir] = contrib
projects[uuid][subdir] = contrib
projects[variable][subdir] = contrib
projects[views][subdir] = contrib
projects[wysiwyg][subdir] = contrib

; Multilingual
projects[i18n][subdir] = contrib
projects[entity_translation][subdir] = contrib
projects[title][subdir] = contrib
; projects[lang_dropdown][subdir] = contrib
; projects[languagefield][subdir] = contrib

; Search modules
; dependencies[] = facetapi
projects[search_api][subdir] = contrib
; dependencies[] = search_api_attachments
; dependencies[] = search_api_et
; dependencies[] = search_api_et_solr
; dependencies[] = search_api_solr
; dependencies[] = search_autocomplete

; Common modules
projects[chosen][subdir] = contrib
; dependencies[] = entity_collection
; dependencies[] = htmlmail
; dependencies[] = mailsystem
; dependencies[] = image_field_caption
; dependencies[] = menu_block
; dependencies[] = migrate
; dependencies[] = r4032login
; dependencies[] = rules
; dependencies[] = views_slideshow

; Development modules
projects[devel][subdir] = contrib
projects[reroute_email][subdir] = contrib

; Rare modules
; dependencies[] = context
; dependencies[] = diff
; dependencies[] = field_group
; dependencies[] = flickr
; dependencies[] = on_the_web
; dependencies[] = linkchecker
; dependencies[] = pathologic
; dependencies[] = views_bulk_operations

; Exotic modules
; dependencies[] = feeds
; dependencies[] = file_entity
; dependencies[] = media
; dependencies[] = tmgmt
; dependencies[] = workbench
; dependencies[] = workbench_access
; dependencies[] = workbench_moderation
