const clear = document.querySelector(".clear");
const dateElement = document.getElementById("date");
const list = document.getElementById("list");
const input = document.getElementById("input");
const CHECK = "fa-check-circle";
const UNCHECK = "fa-circle-thin";
const LINE_THROUGH = "linethrough";

let LIST = [];
let id = 0;

let options = { weekday:'long', month:'short', day:'numeric' };
let today = new Date();

dateElement.innerHTML = today.toLocaleDateString("en-US", options);

document.addEventListener("keyup", function(event){
  if( event.keyCode == 13 ){
    const toDo = input.value;
    
    if(toDo){
      addToDo(toDo, id, false, false)
      LIST.push(
        {
          name: toDo,
          id: id,
          done: false,
          trash: false
        }
        );
      }
      input.value = "";
      id++;
    }
  });
  
  list.addEventListener("click", function(event){
    let element = event.target;
    const elementJob = event.target.attributes.job.value;
    if( elementJob == "complete" ){
      completeTodo( element )
    }else if( elementJob == "delete" ){
      removeTodo( element )
    }
  });

clear.addEventListener('click', function(){
    localStorage.clear();
    location.reload();
})
  
function addToDo( toDo, id, done, trash ){
  if( trash ){ return; }

  const DONE = done ? CHECK : UNCHECK; 
  const LINE = done ? LINE_THROUGH : "";
  const text =  `<li class="item">
                  <i class="fa ${DONE}"" job="complete" id="${id}"></i>
                  <p type="text ${LINE}"> ${toDO} </p>
                  <i class="fa fa-trash-o delete" job="delete" id="${id}"></i>
                </li>`;
  const position = "beforeend";
  list.insertAdjacentHTML("afterbegin", text);
};



function completeTodo( element ){
  element.classList.toggle( CHECK );
  element.classList.toggle( UNCHECK );
  element.parentNode.querySelector(".text").classList.toggle( LINE_THROUGH );
  LIST[element.id].done = LIST[element.id].done ? false: true;
}

function removeTodo( element ){
  element.parentNode.parentNode.removeChild(element.parentNode);
  LIST[element.id].trash = true;
}

let key = localStorage.getItem('key');

function saveTodo( element ){
  localStorage.setItem("TODO", JSON.stringify(LIST));
}

function loadTodo( array ){
  array.forEach(function(item){
    addToDo(item.toDo, item.id, item.done, item.trash)
  });
}



let data = localStorage.getItem("TODO");

if( data ){
  LIST = JSON.parse(data)
  loadTodo( LIST )
  id = LIST.length;
}else{
  LIST = [];
  id = 0;
}