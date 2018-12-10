<?php
/**
 * @file
 * uwwtd_search.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function uwwtd_search_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "facetapi" && $api == "facetapi_defaults") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function uwwtd_search_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_default_search_api_autocomplete_search().
 */
function uwwtd_search_default_search_api_autocomplete_search() {
  $items = array();
  $items['search_api_views_search'] = entity_import('search_api_autocomplete_search', '{
    "machine_name" : "search_api_views_search",
    "name" : "Search",
    "index_id" : "default_node_index",
    "suggester_id" : "server",
    "type" : "search_api_views",
    "enabled" : "1",
    "options" : { "results" : true, "fields" : [] }
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_index().
 */
function uwwtd_search_default_search_api_index() {
  $items = array();
  $items['default_node_index'] = entity_import('search_api_index', '{
    "name" : "Default node index",
    "machine_name" : "default_node_index",
    "description" : "An automatically created search index for indexing node data. Might be configured to specific needs.",
    "server" : "main_search",
    "item_type" : "node",
    "options" : {
      "datasource" : { "bundles" : [
          "agglomeration",
          "big_city",
          "discharge_point",
          "receiving_area",
          "uwwtp"
        ]
      },
      "index_directly" : 1,
      "cron_limit" : "50",
      "data_alter_callbacks" : { "search_api_alter_node_access" : { "status" : 1, "weight" : "0", "settings" : [] } },
      "processors" : {
        "search_api_case_ignore" : { "status" : 1, "weight" : "0", "settings" : { "strings" : 0 } },
        "search_api_html_filter" : {
          "status" : 1,
          "weight" : "10",
          "settings" : {
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\nh2 = 3\\nh3 = 2\\nstrong = 2\\nb = 2\\nem = 1.5\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 1,
          "weight" : "20",
          "settings" : { "spaces" : "[^\\\\p{L}\\\\p{N}]", "ignorable" : "[-]" }
        }
      },
      "fields" : {
        "author" : { "type" : "integer", "entity_type" : "user" },
        "body:value" : { "type" : "text" },
        "changed" : { "type" : "date" },
        "created" : { "type" : "date" },
        "field_aggcompliance" : { "type" : "string" },
        "field_agggenerated" : { "type" : "integer" },
        "field_anneedata" : { "type" : "string" },
        "field_inspireidlocalid" : { "type" : "text" },
        "field_uwwcompliance" : { "type" : "string" },
        "promote" : { "type" : "boolean" },
        "search_api_language" : { "type" : "string" },
        "sticky" : { "type" : "boolean" },
        "title" : { "type" : "text", "boost" : "5.0" },
        "type" : { "type" : "string" },
        "url" : { "type" : "uri" }
      }
    },
    "enabled" : "1",
    "read_only" : "0"
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function uwwtd_search_default_search_api_server() {
  $items = array();
  $items['main_search'] = entity_import('search_api_server', '{
    "name" : "Main search",
    "machine_name" : "main_search",
    "description" : "",
    "class" : "search_api_db_service",
    "options" : {
      "database" : "default:default",
      "min_chars" : "2",
      "partial_matches" : 1,
      "autocomplete" : { "suggest_suffix" : 1, "suggest_words" : 1 },
      "indexes" : { "default_node_index" : {
          "type" : {
            "table" : "search_api_db_default_node_index",
            "column" : "type",
            "type" : "string",
            "boost" : "1.0"
          },
          "title" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "text",
            "boost" : "5.0"
          },
          "promote" : {
            "table" : "search_api_db_default_node_index",
            "column" : "promote",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "sticky" : {
            "table" : "search_api_db_default_node_index",
            "column" : "sticky",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "created" : {
            "table" : "search_api_db_default_node_index",
            "column" : "created",
            "type" : "date",
            "boost" : "1.0"
          },
          "changed" : {
            "table" : "search_api_db_default_node_index",
            "column" : "changed",
            "type" : "date",
            "boost" : "1.0"
          },
          "author" : {
            "table" : "search_api_db_default_node_index",
            "column" : "author",
            "type" : "integer",
            "boost" : "1.0"
          },
          "search_api_language" : {
            "table" : "search_api_db_default_node_index",
            "column" : "search_api_language",
            "type" : "string",
            "boost" : "1.0"
          },
          "body:value" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_anneedata" : {
            "table" : "search_api_db_default_node_index",
            "column" : "field_anneedata",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_inspireidlocalid" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_aggcompliance" : {
            "table" : "search_api_db_default_node_index",
            "column" : "field_aggcompliance",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_agggenerated" : {
            "table" : "search_api_db_default_node_index",
            "column" : "field_agggenerated",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_uwwcompliance" : {
            "table" : "search_api_db_default_node_index",
            "column" : "field_uwwcompliance",
            "type" : "string",
            "boost" : "1.0"
          },
          "url" : {
            "table" : "search_api_db_default_node_index",
            "column" : "url",
            "type" : "uri",
            "boost" : "1.0"
          }
        }
      }
    },
    "enabled" : "1"
  }');
  return $items;
}
