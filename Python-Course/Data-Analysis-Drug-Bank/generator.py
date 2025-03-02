import xml.etree.ElementTree as ET
import random
import copy

# Parse the input XML file using ET.parse
tree = ET.parse('drugbank_partial.xml')
root = tree.getroot()

# Determine the namespace if present
namespace = ''
if '}' in root.tag:
    namespace = root.tag.split('}')[0] + '}'

# Write the XML declaration and root element to the output file
with open('drugbank_partial_and_generated.xml', 'wb') as f:
    f.write(b'<?xml version="1.0" encoding="UTF-8"?>\n')
    f.write(b'<drugbank>\n')

tag_structure = {}

# Process each <drug> element in the input XML file
for drug in root.findall(f"{namespace}drug"):
    drugbank_id_elem = drug.find(f"{namespace}drugbank-id[@primary='true']")
    for child in drug:
        if child.tag == f"{namespace}drugbank-id":
            continue
        if child.tag not in tag_structure:
            tag_structure[child.tag] = {'elements': []}
        tag_structure[child.tag]['elements'].append(copy.deepcopy(child))
    # Write the processed drug element to the output file
    with open('drugbank_partial_and_generated.xml'
, 'ab') as f:
        f.write(ET.tostring(drug, encoding='utf-8'))
        f.write(b'\n')

# Function to generate a new drug element
def generate_new_drug(tag_structure, new_id):
    new_drug = ET.Element(f'{namespace}drug')
    drugbank_id = ET.SubElement(new_drug, f'{namespace}drugbank-id', {'primary': 'true'})
    drugbank_id.text = f'DB{new_id:05d}'
    for tag, details in tag_structure.items():
        chosen_element = copy.deepcopy(random.choice(details['elements']))
        new_drug.append(chosen_element)
    return new_drug

# Generate and write new drug elements to the output file
with open('drugbank_partial_and_generated.xml', 'ab') as f:
    print('Generating new drugs')
    for i in range(109, 500):
        new_drug = generate_new_drug(tag_structure, i)
        f.write(ET.tostring(new_drug, encoding='utf-8'))
        f.write(b'\n')

# Close the root element in the output file
with open('drugbank_partial_and_generated.xml', 'ab') as f:
    f.write(b'</drugbank>\n')

print(f'Original drugs and new drugs saved to {'drugbank_partial_and_generated.xml'}')