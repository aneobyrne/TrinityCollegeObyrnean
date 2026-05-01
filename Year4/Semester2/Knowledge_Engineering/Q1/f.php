<?php

// Step 1: Get parameters from URL
$file = $_GET['f'] ?? null;
$query = $_GET['q'] ?? null;
$xsltFile = $_GET['t'] ?? null;   // XSLT parameter

// Step 2: Validate input
if ($file === null || $query === null) {
    http_response_code(400);
    echo "Missing parameters";
    exit;
}

// Step 3: Check XML file exists
if (!file_exists($file)) {
    http_response_code(404);
    echo "File not found";
    exit;
}

// Step 4: Load XML
$xml = new DOMDocument();
$xml->load($file);

// Step 5: Evaluate XPath
$xpath = new DOMXPath($xml);
$resultNodes = $xpath->query($query);

if ($resultNodes === false) {
    http_response_code(400);
    echo "Invalid XPath expression";
    exit;
}

if ($resultNodes->length === 0) {
    http_response_code(204);
    echo "No matching results";
    exit;
}

// Step 6: Convert result into XML string
$outputXML = "<results>";
foreach ($resultNodes as $node) {
    $outputXML .= $xml->saveXML($node);
}
$outputXML .= "</results>";

// Step 7: OPTIONAL XSLT transformation
if ($xsltFile !== null && file_exists($xsltFile)) {

    $xsl = new DOMDocument();
    $xsl->load($xsltFile);

    $proc = new XSLTProcessor();
    $proc->importStylesheet($xsl);

    $result = $proc->transformToXML(new SimpleXMLElement($outputXML));

    header("Content-Type: text/html");
    echo $result;

} else {

    // Default: return XML
    header("Content-Type: application/xml");
    echo $outputXML;
}

?>