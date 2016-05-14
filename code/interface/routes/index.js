
/*
 * GET home page.
 */

var co2=[];
var count=0;
var request=require('request');
exports.index = function(req, res){
  res.render('index', { title: 'Air Purity Sensor' });
};
 
exports.updateCo2Value=function(req,res){
	var reading=req.query.co2Value;
	console.log("val"+reading);
	if(reading!="")
		{
	co2[count]=reading;
	//co2[0]="5";
	count++;
	console.log("Co2 Value "+JSON.stringify(co2));
	if(reading>1500){
		on();
	}else{
		off();
	}
	
	res.render('index', { title: 'Air Purity Sensor', co2:co2});
		}
	else{
		res.render('index', { title: 'Air Purity Sensor', co2:co2});
	}
}


function on(){
request("http://192.168.1.3/digital/5/1",function(error,response,body){
	console.log(body);
})	
};

function off(){
	request("http://192.168.1.3/digital/5/0",function(error,response,body){
		console.log(body);
	})	
	};

