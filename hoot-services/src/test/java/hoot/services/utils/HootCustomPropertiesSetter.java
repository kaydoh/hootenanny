package hoot.services.utils;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.Properties;

import hoot.services.HootProperties;


/**
 * Created by dmylov on 5/10/16.
 */
public class HootCustomPropertiesSetter {

    public static void setProperties(Properties newProperties) throws Exception {
        Field field = HootProperties.class.getDeclaredField("properties");
        field.setAccessible(true);

        Field modifiersField = Field.class.getDeclaredField("modifiers");
        modifiersField.setAccessible(true);
        modifiersField.setInt(field, field.getModifiers() & ~Modifier.FINAL);

        field.set(null, newProperties);
    }

    public static void setProperty(String key, String value) throws Exception {
        Field propertiesField = HootProperties.class.getDeclaredField("properties");
        propertiesField.setAccessible(true);

        Properties properties = (Properties) propertiesField.get(null);
        properties.setProperty(key, value);
    }
}
