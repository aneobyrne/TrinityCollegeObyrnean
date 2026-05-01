<?php

$file = $_GET['f'] ?? null;
$query = $_GET['q'] ?? null;

if ($file === null || $query === null) {
    http_response_code(400);
    echo "Missing parameters";
    exit;
}

if (!file_exists($file)) {
    http_response_code(404);
    echo "File not found";
    exit;
}

// Load XML with DOM 
$dom = new DOMDocument();
$dom->load($file);

// Validate against XSD
if (!$dom->schemaValidate("b.xsd")) {
    http_response_code(400);
    echo "XML does not conform to schema";
    exit;
}

// Convert to SimpleXML for XPath
$xml = simplexml_import_dom($dom);

// Run XPath
$result = $xml->xpath($query);

if ($result === false) {
    http_response_code(400);
    echo "Invalid XPath";
    exit;
}

if (empty($result)) {
    http_response_code(204);
    echo "No results";
    exit;
}

// Output Valid XML
header("Content-Type: application/xml; charset=UTF-8");

echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
echo "<results>";

foreach ($result as $node) {
    echo $node->asXML();
}

echo "</results>";

?>