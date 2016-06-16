import tw_config as config
import json
import os

def read_tests(file_path):
    with open(file_path,'r') as json_data:
        data = json.loads(json_data.read())
    return data

def run_tests(tests):
    for test in tests:
        print  test['Name']
        print  test['Description']
        print "running..."
        print (os.popen('sudo '+ test['executable']).read())

tests_path = "smoke_tests.json"
run_tests (read_tests(tests_path))

