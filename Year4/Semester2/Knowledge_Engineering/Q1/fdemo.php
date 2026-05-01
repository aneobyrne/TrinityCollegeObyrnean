<?php

$file = $_GET['f'] ?? null;
$query = $_GET['q'] ?? null;
$xsltFile = $_GET['t'] ?? null;   // Optional XSLT file

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

// Load XML
$dom = new DOMDocument();
$dom->load($file);

// Validate against XSD 
if (!$dom->schemaValidate("b.xsd")) {
    http_response_code(400);
    echo "XML does not conform to schema";
    exit;
}

// XPath
$xpath = new DOMXPath($dom);
$resultNodes = $xpath->query($query);

if ($resultNodes === false) {
    http_response_code(400);
    echo "Invalid XPath";
    exit;
}

if ($resultNodes->length === 0) {
    http_response_code(204);
    echo "No results";
    exit;
}

// Build result XML 
$outputXML = "<results>";
foreach ($resultNodes as $node) {
    $outputXML .= $dom->saveXML($node);
}
$outputXML .= "</results>";

// XSLT TRANSFORMATION 

if ($xsltFile !== null && file_exists($xsltFile)) {

    $xsl = new DOMDocument();
    $xsl->load($xsltFile);

    $proc = new XSLTProcessor();
    $proc->importStylesheet($xsl);

    // convert XML string to SimpleXML for transformation
    $xmlInput = new SimpleXMLElement($outputXML);

    $result = $proc->transformToXML($xmlInput);

    header("Content-Type: text/html; charset=UTF-8");
    echo $result;

} else {

    // DEFAULT: return XML
    header("Content-Type: application/xml; charset=UTF-8");
    echo $outputXML;
}

?>