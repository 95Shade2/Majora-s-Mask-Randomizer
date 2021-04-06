Shown = false;
Searching = false;

function Cover(id) {
	item_id = "item_give_" + id;
	classes = document.getElementById(item_id).classList;

	if (classes.contains("cover")) {
		document.getElementById(item_id).classList.remove("cover");
	}
	else {
		document.getElementById(item_id).classList.add("cover");
	}
}

function Search() {
	text = document.getElementById("search_items").value;
	text = text.toLowerCase();

	if (text != "") {
		Searching = true;

		for (i = 0; i < Total_Items; i++) {
			item = document.getElementById("item_" + i);
			item_source = document.getElementById("item_source_" + i);
			item_source_text = item_source.innerHTML.toLowerCase();
			item_give_text = "";

			if (Shown) {
				item_give_text = document.getElementById("item_give_" + i).innerHTML.toLowerCase();
			}

			if (item_source_text.includes(text) || item_give_text.includes(text)) {
				item.classList.remove("hide");
			}
			else {
				item.classList.add("hide");
			}
		}
	}
	else {
		if (Searching) {
			Searching = false;

			for (i = 0; i < Total_Items; i++) {
				item = document.getElementById("item_" + i);
				item.classList.remove("hide");
			}
		}
	}
}

function Reveal_All() {
	if (!Shown) {
		for (i = 0; i < Total_Items; i++) {
			item_give = document.getElementById("item_give_" + i);

			if (item_give.classList.contains("cover")) {
				item_give.classList.remove("cover");
			}

			Shown = true;
			document.getElementById("reveal_button").innerHTML = "Hide All";
		}
	}
	else {for (i = 0; i < Total_Items; i++) {
			item_give = document.getElementById("item_give_" + i);

			if (!item_give.classList.contains("cover")) {
				item_give.classList.add("cover");
			}

			Shown = false;
			document.getElementById("reveal_button").innerHTML = "Reveal All";
		}
	}
}

//run the search function every 100 ms
function Search_Loop() {
	Search();
	setTimeout(function() {
		Search_Loop();
	}, 100);
}

Search_Loop();