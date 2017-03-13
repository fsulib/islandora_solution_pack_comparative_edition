#!/usr/bin/php

<?php

//$url = 'http://146.201.32.3/collatex/collate';
$url = 'https://collatex.net/demo/collate';

$test_text = '{
  "witnesses" : [
    {
      "id" : "A",
      "content" : "A black cat in a black basket"
    },
    {
      "id" : "B",
      "content" : "A black cat in a black basket"
    },
    {
      "id" : "C",
      "content" : "A striped cat in a black basket"
    },
    {
      "id" : "D",
      "content" : "A striped cat in a white basket"
    }
  ]
}';

$curl = curl_init($url);
curl_setopt($curl, CURLOPT_HEADER, false);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($curl, CURLOPT_HTTPHEADER, array(
  "Content-type: application/json",
  "Accept: application/json"
));
curl_setopt($curl, CURLOPT_POST, true);
curl_setopt($curl, CURLOPT_POSTFIELDS, $test_text);

$json_response = curl_exec($curl);

$status = curl_getinfo($curl, CURLINFO_HTTP_CODE);

if ( $status != 200 ) {
    die("Error: call to URL $url failed with status $status, response $json_response, curl_error " . curl_error($curl) . ", curl_errno " . curl_errno($curl));
}


curl_close($curl);

$response = json_decode($json_response, true);

print_r($response);

?>
