drush php-eval '
$node = new stdClass(); 
$node->type = "article"; 
$node->title = "Arduino test"; 
$filepath = drupal_realpath("/vagrant/webcam/00000001.png");
$file = (object) array(
  "uid" => 1,
  "uri" => $filepath,
  "filemime" => file_get_mimetype($filepath),
  "status" => 1,
);
$file = file_copy($file, "public://");
$node->field_image[LANGUAGE_NONE][0] = (array)$file;
node_save($node);';
