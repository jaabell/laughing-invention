all:

	# run all the tests and print the results on both the command line and file 
	@script -q -c "./scripts/test.sh" verification_results.log

	# statistics
	# count the test cases and print the results on both the command line and file 
	@python ./scripts/count.py verification_results.log  
	@python ./scripts/count.py verification_results.log  >> verification_results.log



	

update:
	# update the previous results to current results
	./scripts/update.sh
clean:
	# clean the results
	find -name "*.feioutput" -delete

# show the results on html. 
	# @aha -f verification_results.log > verification_results.html 
# @cp verification_results.html ~/public_html/essi_autotest/

