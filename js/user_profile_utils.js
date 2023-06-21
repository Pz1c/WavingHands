function getLastActivityText(last_activity_in_sec) {
    if (!(last_activity_in_sec > 0)) {
        return "";
    }
    var date = new Date();
    var diff = Math.floor(date.getTime()/1000) - last_activity_in_sec;
    console.log("UPU.getLastActivityText", Math.floor(date.getTime()/1000), last_activity_in_sec, date.getTime(), diff);

    if (Math.floor(diff / (24 * 60 * 60)) > 0) {
        return "Last active ~" + Math.floor(diff / (24 * 60 * 60)) + " days ago";
    } else if (Math.floor(diff / (60 * 60)) > 0) {
        return "Last active ~" + Math.floor(diff / (60 * 60)) + " hours ago";
    } else if (Math.floor(diff / (60)) > 0) {
        return "Last active ~" + Math.floor(diff / (60)) + " minutes ago";
    } else {
        return "Active right now";
    }
}
