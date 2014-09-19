#!/usr/bin/python

from sys import argv, stdout, exit
from os.path import dirname
from xml.etree.ElementTree import parse

if len(argv) != 2:
    stdout.write('Usage: %s labelling_path\n' % argv[0])
    exit(1)

labelling_path = argv[1]

batch_tag = parse(labelling_path).getroot()

for photo_tag in batch_tag.findall('Photo'):
    bboxes = photo_tag.find('BBoxes')
    if bboxes is None:
        bboxes = []
    else:
        bboxes = bboxes.findall('BBox')

    for bbox_tag in bboxes:
        bbox_id = int(bbox_tag.attrib['ID'])
        x = int(bbox_tag.find('X').text)
        y = int(bbox_tag.find('Y').text)
        w = int(bbox_tag.find('W').text)
        h = int(bbox_tag.find('H').text)

        stdout.write('%s %d %d %d %d %d\n' % (photo_tag.find('File').text, x, y, w, h, bbox_id))
