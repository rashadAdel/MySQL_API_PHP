<?php
require 'src/db.inc';

$db = new db();
$uri = urldecode($_SERVER['REQUEST_URI']);
$tableName = explode("/", $uri)[1];

switch ($_SERVER['REQUEST_METHOD']) {

    case 'GET':
        if ($uri == "/") {
            echo $db->query("SHOW TABLES", null, 7);
        } else if (stristr($uri, "/query")) {
            $query = end(explode("/", $uri));
            echo $db->query($query);
        } else {
            $where = end(explode("/", $uri));
            echo $db->query($tableName, $where, 2);
        }
        break;
    //insert
    case 'POST':
        $body = file_get_contents('php://input');
        echo $db->insert($tableName, $body);
        break;
    //update
    case 'PUT':
        $body = json_decode( file_get_contents('php://input'));
        echo $db->update($tableName, $body->data, $body->where);
        break;

    //delete
    case 'DELETE':
        $where = end(explode("/", $uri));
        echo "deleted ". $db->delete($tableName, $where). " Rows";

        break;

    default:

        break;
}
