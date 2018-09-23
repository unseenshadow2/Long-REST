function test(id)
{
	var toChange = document.getElementById(id);
	toChange.innerHTML = "The test function worked...";
	toChange.className = "test";
}