<?php

// Step 1: Get parameters from URL
$file = $_GET['f'] ?? null;   // XML file name
$query = $_GET['q'] ?? null;  // XPath expression

// Step 2: Validate input
if ($file === null || $query === null) {
    http_response_code(400);
    echo "Missing parameters";
    exit;
}

// Step 3: Check file exists
if (!file_exists($file)) {
    http_response_code(404);
    echo "File not found";
    exit;
}

// Step 4: Load XML file
$xml = simplexml_load_file($file);
if ($xml === false) {
    http_response_code(500);
    echo "Error loading XML file";
    exit;
}

// Step 5: Execute XPath query
$result = $xml->xpath($query);
if ($result === false) {
    http_response_code(400);
    echo "Invalid XPath expression";
    exit;
}

// Step 6: Handle empty result
if (empty($result)) {
    http_response_code(204);
    echo "No matching results";
    exit;
}

// Step 7: Return result as XML
header("Content-Type: application/xml");

foreach ($result as $node) {
    echo $node->asXML();
}

?>