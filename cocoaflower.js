window.onload = function(){
  var res = 0;
  var percent = 0;
  var negative_count_2 = 0;
  var cocoa = 1;
  var all = 1;
  var per2 = document.getElementById('p2');
  var neg2button = document.getElementById('neg2button');
  var pos2button = document.getElementById('pos2button');
  var img = document.getElementById('img');
  var num1 = 0;
  var sumnum = 0;

  $.getJSON("num.json", function(data){
    cocoa = data.cocoa;
    all = data.all;
    res = Math.floor((cocoa/all)*100);
    per2.innerHTML = res + '%';
    flower_change(res);
  });

  function flower_change(per){
    if (per < 20){
      img.src = "flower0.png";
    } else if (20 <= per && per < 40){
      img.src = "flower1.png";
    } else if (40 <= per && per < 60){
      img.src = "flower2.png";
    } else if (60 <= per && per < 80){
      img.src = "flower3.png";
    } else if (80 <= per && per <= 100){
      img.src = "flower4.png";
    } 
  }

  // $.getJSON("num.json", function(data){
  //   console.log(data.value);
  // });
  
  // neg2button.addEventListener('click', () => {
  //   negative_count_2++;
  //   all_count_2++;
  //   res = Math.floor((positive_count_2/all_count_2)*100)
  //   per2.innerHTML = res + '%';
  //   flower_change(res)
  // });

  // pos2button.addEventListener('click', () => {
  //   sumnum = sumnum + num1
  //   positive_count_2++;
  //   all_count_2++;
  //   res = Math.floor((positive_count_2/all_count_2)*100)
  //   per2.innerHTML = sumnum + '%';
  //   flower_change(res)
  // });
}