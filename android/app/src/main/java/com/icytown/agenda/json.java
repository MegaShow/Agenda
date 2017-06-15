package com.icytown.agenda;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

/**
 * Created by Mega on 2017/6/16.
 */

public class json {
    public static int getId(String str) {
        JSONTokener jsonTokener = new JSONTokener(str);
        JSONObject obj;
        try {
            obj = (JSONObject) jsonTokener.nextValue();
            return obj.getInt("id");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return 0;
    }

    public static String getStatus(String str) {
        JSONTokener jsonTokener = new JSONTokener(str);
        JSONObject obj;
        try {
            obj = (JSONObject) jsonTokener.nextValue();
            return obj.getString("status");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return "";
    }
}
