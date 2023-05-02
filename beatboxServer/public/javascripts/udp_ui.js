"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.

var socket = io.connect();
var $inputvol = $(this).parent().find('input');
var $inputbpm;
var timeout;
var timeout = true
var volval = 0;
var val = setInterval(function(){
	socket.emit("getvol", "getvol");
	socket.emit('proc', 'uptime');

	socket.emit("getbeat", "getbeat");
	socket.emit("getbpm", "getbpm");
	timeout = (setTimeout(function(){
		
		$('#error-box1').show();
		$('#error-box').hide();
	},1000))

	
},1000);



$(document).ready(function() {
	

	


	$('#btnCount').click(function(){
		sendCommandViaUDP("count");
	});
	$('#btnLength').click(function(){
		sendCommandViaUDP("length");
	});
	$('#btnHistory').click(function(){
		sendCommandViaUDP("history");
	});
	$('#btnStop').click(function(){
		sendCommandViaUDP("stop\n");
	});

	$("#btnR0").click(function(){
		sendCommandViaUDP("beat0\n");
	});

	$("#btnR1").click(function(){
		sendCommandViaUDP("beat1\n");
	});

	$("#btnR2").click(function(){
		sendCommandViaUDP("beat2\n");
	});

	$("#btnS1").click(function(){
		sendCommandViaUDP("Hi-Hat");
	});
	$("#btnS2").click(function(){
		sendCommandViaUDP("Snare");
	});
	$("#btnS3").click(function(){
		sendCommandViaUDP("Base");
	});

	$('.minus').click(function () {
		var $input = $(this).parent().find('input');
		var count = parseInt($input.val()) - 5;
		count = count < 0 ? 0 : count;
		let text = "vol" +count;
		console.log(text);
		sendCommandViaUDP(text);
		$input.val(count);
		$input.change();
	});
	$('.plus').click(function () {
		var $input = $(this).parent().find('input');
		var count = parseInt($input.val()) + 5;
		count = count > 100 ? 100 : count;
		let text = "vol" +count;
		sendCommandViaUDP(text);
		$input.val(count);
		$input.change();
		return false;
	})

	$('.minus1').click(function () {

		var count = parseInt($inputbpm.val()) - 5;
		count = count < 0 ? 40 : count;
		let text = "tem" +count;
		console.log(text);
		sendCommandViaUDP(text);
		$inputbpm.val(count);
		$inputbpm.change();
	});
	
	$('.plus1').click(function () {
		
		var count = parseInt($inputbpm.val()) + 5;
		count = count > 300 ? 300: count;
		let text = "tem" +count;
		sendCommandViaUDP(text);
		$inputbpm.val(count);
		$inputbpm.change();
		
	});

	

	


	socket.on("BBTimeout",function(result){
		$('#error-box').show();
		$('#error-box1').hide();

		clearTimeout(timeout);
	})

	socket.on('fileContents',function(result){
		var contents = result.contents;
		var array = contents.split(" ")
		var uptime = parseFloat(array[0]);
		var hour = parseInt(uptime/3600);
		var mintue = parseInt((uptime % 3600)/60)
		var seconds = parseInt(uptime % 3600 % 60)
		var hms = hour + ":" + mintue + ":" + seconds
		$("#time").html($('<span></span>').text(hms));
	})

	









	
	socket.on('commandReply', function(result) {
		var newDiv = $('<code></code>')
			.text(result)
			.wrapInner("<div></div>");
		$('#messages').append(newDiv);
		$('#messages').scrollTop($('#messages').prop('scrollHeight'));
	});

	socket.on("beatReply", function(result){
		clearTimeout(timeout);
		$('#error-box1').hide();
		$('#error-box').hide();
		$("#currentBeat").html($('<span></span>').text(result));
	});

	socket.on("volReply", function(result){

		$('.vol').each(function () {
			var $input = $(this).parent().find('input');
			
			
			$input.val(result);
			$input.change();
			
		});

	});
	//$('#error-box').show();

	//$('#box1').hide();
	
	socket.on("bpmReply", function(result){

		$('.bpm').each(function () {
			var $input = $(this).parent().find('input');
			$inputbpm = $input;

			$input.val(result);
			$input.change();
			
		});

	});
	


	
});

function sendCommandViaUDP(message) {
	socket.emit('daUdpCommand', message);
};

