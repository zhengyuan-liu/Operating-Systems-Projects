# NAME: Zhengyuan Liu
# EMAIL: zhengyuanliu@ucla.edu
#
# smoke_test_sh: script to do the smoke tests

let errors=0

./lab4b --wrong &> /dev/null
rc=$?
if [ $rc -eq 1 ]
then
	echo "Passed: program ended with the correct exit code 1."
else
	echo "Error: program ended with the wrong exit code!"
	let errors+=1
fi

./lab4b --period=2 --scale=C --log="tmp.txt" <<-EOF
SCALE=F
PERIOD=5
START
STOP
LOG test
OFF
EOF
rc=$?
if [ $rc -ne 0 ]
then
	echo "Error: program ended with the wrong exit code 1!"
	let errors+=1
else
	echo "Passed: program ended with the correct exit code 0."
fi
if [ ! -s tmp.txt ]
then
	echo "Error: program did not create a logfile"
	let errors+=1
else
	echo "Passed: program created a logfile"
	for c in SCALE=F PERIOD=5 START STOP OFF SHUTDOWN "LOG test"
	do
		grep "$c" tmp.txt > /dev/null
		if [ $? -ne 0 ]
		then
			echo "Error: the logfile did not log $c command"
			let errors+=1
		else
			echo "Passed: program recognized and logged $c command"
		fi
	done
fi

if [ $errors -eq 0 ]
then
	echo "All smoke test cases passed!"
else
	echo "Smoke tests failed with $errors errors."
fi

rm -f tmp.txt
